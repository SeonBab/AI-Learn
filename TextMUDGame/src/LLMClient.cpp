#include "LLMClient.h"

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#endif

#include <sstream>
#include <stdexcept>
#include <utility>

namespace
{
    int hexDigitToValue(char character)
    {
        if (character >= '0' && character <= '9')
        {
            return character - '0';
        }

        if (character >= 'a' && character <= 'f')
        {
            return character - 'a' + 10;
        }

        if (character >= 'A' && character <= 'F')
        {
            return character - 'A' + 10;
        }

        return -1;
    }

    bool tryReadUnicodeEscape(const std::string& text, std::size_t offset, unsigned int& codePoint)
    {
        if (offset + 4 > text.size())
        {
            return false;
        }

        codePoint = 0;
        for (std::size_t index = 0; index < 4; ++index)
        {
            const int value = hexDigitToValue(text[offset + index]);
            if (value < 0)
            {
                return false;
            }

            codePoint = (codePoint << 4) | static_cast<unsigned int>(value);
        }

        return true;
    }

    void appendUtf8(std::string& text, unsigned int codePoint)
    {
        if (codePoint <= 0x7F)
        {
            text.push_back(static_cast<char>(codePoint));
            return;
        }

        if (codePoint <= 0x7FF)
        {
            text.push_back(static_cast<char>(0xC0 | (codePoint >> 6)));
            text.push_back(static_cast<char>(0x80 | (codePoint & 0x3F)));
            return;
        }

        if (codePoint <= 0xFFFF)
        {
            text.push_back(static_cast<char>(0xE0 | (codePoint >> 12)));
            text.push_back(static_cast<char>(0x80 | ((codePoint >> 6) & 0x3F)));
            text.push_back(static_cast<char>(0x80 | (codePoint & 0x3F)));
            return;
        }

        text.push_back(static_cast<char>(0xF0 | (codePoint >> 18)));
        text.push_back(static_cast<char>(0x80 | ((codePoint >> 12) & 0x3F)));
        text.push_back(static_cast<char>(0x80 | ((codePoint >> 6) & 0x3F)));
        text.push_back(static_cast<char>(0x80 | (codePoint & 0x3F)));
    }

    std::string escapeJsonString(const std::string& text)
    {
        std::ostringstream stream;

        for (char character : text)
        {
            switch (character)
            {
            case '\\':
                stream << "\\\\";
                break;
            case '"':
                stream << "\\\"";
                break;
            case '\n':
                stream << "\\n";
                break;
            case '\r':
                stream << "\\r";
                break;
            case '\t':
                stream << "\\t";
                break;
            default:
                stream << character;
                break;
            }
        }

        return stream.str();
    }

    std::string unescapeJsonString(const std::string& text)
    {
        std::ostringstream stream;

        for (std::size_t index = 0; index < text.size(); ++index)
        {
            char character = text[index];
            if (character != '\\' || index + 1 >= text.size())
            {
                stream << character;
                continue;
            }

            char escaped = text[++index];
            switch (escaped)
            {
            case 'n':
                stream << '\n';
                break;
            case 'r':
                stream << '\r';
                break;
            case 't':
                stream << '\t';
                break;
            case '"':
                stream << '"';
                break;
            case '\\':
                stream << '\\';
                break;
            case '/':
                stream << '/';
                break;
            case 'u':
            {
                unsigned int codePoint = 0;
                if (!tryReadUnicodeEscape(text, index + 1, codePoint))
                {
                    stream << 'u';
                    break;
                }

                index += 4;

                if (codePoint >= 0xD800 && codePoint <= 0xDBFF)
                {
                    if (index + 6 < text.size() && text[index + 1] == '\\' && text[index + 2] == 'u')
                    {
                        unsigned int lowSurrogate = 0;
                        if (tryReadUnicodeEscape(text, index + 3, lowSurrogate) &&
                            lowSurrogate >= 0xDC00 &&
                            lowSurrogate <= 0xDFFF)
                        {
                            codePoint = 0x10000 +
                                ((codePoint - 0xD800) << 10) +
                                (lowSurrogate - 0xDC00);
                            index += 6;
                        }
                    }
                }

                std::string utf8Text;
                appendUtf8(utf8Text, codePoint);
                stream << utf8Text;
                break;
            }
            default:
                stream << escaped;
                break;
            }
        }

        return stream.str();
    }

#ifdef _WIN32
    class WinSockSession
    {
    public:
        WinSockSession()
        {
            WSADATA data;
            if (WSAStartup(MAKEWORD(2, 2), &data) != 0)
            {
                throw std::runtime_error("WSAStartup failed.");
            }
        }

        ~WinSockSession()
        {
            WSACleanup();
        }
    };
#endif
}

LLMClient::LLMClient() = default;

LLMClient::LLMClient(std::string host, int port, std::string modelName, bool mockMode)
    : mockMode_(mockMode),
      host_(std::move(host)),
      port_(port),
      modelName_(std::move(modelName))
{
}

bool LLMClient::initialize()
{
    if (mockMode_)
    {
        available_ = true;
        return true;
    }

    try
    {
        postGenerate("준비", keepAlive_);
        available_ = true;
    }
    catch (...)
    {
        available_ = false;
    }

    return available_;
}

std::string LLMClient::generateResponse(const std::string& prompt) const
{
    if (mockMode_)
    {
        return generateMockResponse(prompt);
    }

    if (!available_)
    {
        return "";
    }

    try
    {
        return generateOllamaResponse(prompt);
    }
    catch (...)
    {
        return "";
    }
}

std::string LLMClient::generate(const std::string& prompt) const
{
    return generateResponse(prompt);
}

void LLMClient::shutdown()
{
    if (mockMode_ || !available_)
    {
        return;
    }

    try
    {
        postGenerate("", "0");
    }
    catch (...)
    {
    }

    available_ = false;
}

bool LLMClient::isAvailable() const
{
    return available_;
}

bool LLMClient::isMockMode() const
{
    return mockMode_;
}

const std::string& LLMClient::getModelName() const
{
    return modelName_;
}

std::string LLMClient::generateMockResponse(const std::string& prompt) const
{
    (void)prompt;
    return "그 질문에 대해선 조심스럽게 생각해보는 게 좋겠군.";
}

std::string LLMClient::generateOllamaResponse(const std::string& prompt) const
{
    std::string response = postGenerate(prompt, keepAlive_);
    response = removeThinkTags(response);
    response = trimWhitespace(response);
    response = trimResponseLength(response);
    return response;
}

std::string LLMClient::postGenerate(const std::string& prompt, const std::string& keepAliveValue) const
{
    const std::string body = buildRequestBody(prompt, keepAliveValue);
    const std::string httpResponse = sendHttpPost("/api/generate", body);
    return extractResponseText(httpResponse);
}

std::string LLMClient::buildRequestBody(const std::string& prompt, const std::string& keepAliveValue) const
{
    std::ostringstream body;
    body << "{"
         << "\"model\":\"" << escapeJsonString(modelName_) << "\","
         << "\"prompt\":\"" << escapeJsonString(prompt) << "\","
         << "\"stream\":false,"
         << "\"keep_alive\":";

    if (keepAliveValue == "0")
    {
        body << 0;
    }
    else
    {
        body << "\"" << escapeJsonString(keepAliveValue) << "\"";
    }

    body << "}";
    return body.str();
}

std::string LLMClient::sendHttpPost(const std::string& path, const std::string& body) const
{
#ifndef _WIN32
    throw std::runtime_error("This learning sample currently implements the Ollama HTTP client for Windows only.");
#else
    WinSockSession session;

    addrinfo hints{};
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    addrinfo* result = nullptr;
    const std::string portText = std::to_string(port_);
    if (getaddrinfo(host_.c_str(), portText.c_str(), &hints, &result) != 0)
    {
        throw std::runtime_error("Could not resolve Ollama host.");
    }

    SOCKET socketHandle = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (socketHandle == INVALID_SOCKET)
    {
        freeaddrinfo(result);
        throw std::runtime_error("Could not create socket.");
    }

    const DWORD readTimeoutMs = static_cast<DWORD>(readTimeoutSeconds_ * 1000);
    const DWORD writeTimeoutMs = static_cast<DWORD>(writeTimeoutSeconds_ * 1000);
    setsockopt(socketHandle, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<const char*>(&readTimeoutMs), sizeof(readTimeoutMs));
    setsockopt(socketHandle, SOL_SOCKET, SO_SNDTIMEO, reinterpret_cast<const char*>(&writeTimeoutMs), sizeof(writeTimeoutMs));

    if (connect(socketHandle, result->ai_addr, static_cast<int>(result->ai_addrlen)) == SOCKET_ERROR)
    {
        freeaddrinfo(result);
        closesocket(socketHandle);
        throw std::runtime_error("Could not connect to Ollama.");
    }

    freeaddrinfo(result);

    std::ostringstream request;
    request << "POST " << path << " HTTP/1.1\r\n"
            << "Host: " << host_ << ":" << port_ << "\r\n"
            << "Content-Type: application/json; charset=utf-8\r\n"
            << "Content-Length: " << body.size() << "\r\n"
            << "Connection: close\r\n"
            << "\r\n"
            << body;

    const std::string requestText = request.str();
    const int sent = send(socketHandle, requestText.c_str(), static_cast<int>(requestText.size()), 0);
    if (sent == SOCKET_ERROR)
    {
        closesocket(socketHandle);
        throw std::runtime_error("Could not send request to Ollama.");
    }

    std::string response;
    char buffer[4096];
    int received = 0;
    while ((received = recv(socketHandle, buffer, sizeof(buffer), 0)) > 0)
    {
        response.append(buffer, received);
    }

    closesocket(socketHandle);
    return response;
#endif
}

std::string LLMClient::extractResponseText(const std::string& httpResponse) const
{
    const std::string headerSeparator = "\r\n\r\n";
    const std::size_t bodyStart = httpResponse.find(headerSeparator);
    if (bodyStart == std::string::npos)
    {
        throw std::runtime_error("Ollama returned an invalid HTTP response.");
    }

    const std::string body = httpResponse.substr(bodyStart + headerSeparator.size());
    const std::string key = "\"response\":\"";
    const std::size_t responseStart = body.find(key);
    if (responseStart == std::string::npos)
    {
        throw std::runtime_error("Ollama response did not contain a response field.");
    }

    const std::size_t valueStart = responseStart + key.size();
    std::string encodedText;
    bool isEscaped = false;

    for (std::size_t index = valueStart; index < body.size(); ++index)
    {
        char character = body[index];
        if (isEscaped)
        {
            encodedText.push_back('\\');
            encodedText.push_back(character);
            isEscaped = false;
            continue;
        }

        if (character == '\\')
        {
            isEscaped = true;
            continue;
        }

        if (character == '"')
        {
            return unescapeJsonString(encodedText);
        }

        encodedText.push_back(character);
    }

    throw std::runtime_error("Ollama response field was not closed.");
}

std::string LLMClient::removeThinkTags(const std::string& text) const
{
    std::string result = text;
    const std::string openTag = "<think>";
    const std::string closeTag = "</think>";

    while (true)
    {
        const std::size_t open = result.find(openTag);
        if (open == std::string::npos)
        {
            break;
        }

        const std::size_t close = result.find(closeTag, open + openTag.size());
        if (close == std::string::npos)
        {
            result.erase(open);
            break;
        }

        result.erase(open, close + closeTag.size() - open);
    }

    return result;
}

std::string LLMClient::trimResponseLength(const std::string& text) const
{
    if (text.size() <= maxResponseLength_)
    {
        return text;
    }

    return text.substr(0, maxResponseLength_);
}

std::string LLMClient::trimWhitespace(const std::string& text) const
{
    const std::size_t first = text.find_first_not_of(" \t\r\n");
    if (first == std::string::npos)
    {
        return "";
    }

    const std::size_t last = text.find_last_not_of(" \t\r\n");
    return text.substr(first, last - first + 1);
}
