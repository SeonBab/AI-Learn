#include "OllamaClient.h"

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#endif

#include <cctype>
#include <stdexcept>
#include <sstream>
#include <utility>

namespace
{
    int HexDigitToValue(char Character)
    {
        if (Character >= '0' && Character <= '9')
        {
            return Character - '0';
        }

        if (Character >= 'a' && Character <= 'f')
        {
            return Character - 'a' + 10;
        }

        if (Character >= 'A' && Character <= 'F')
        {
            return Character - 'A' + 10;
        }

        return -1;
    }

    bool TryReadUnicodeEscape(const std::string& Text, std::size_t Offset, unsigned int& CodePoint)
    {
        if (Offset + 4 > Text.size())
        {
            return false;
        }

        CodePoint = 0;
        for (std::size_t Index = 0; Index < 4; ++Index)
        {
            const int Value = HexDigitToValue(Text[Offset + Index]);
            if (Value < 0)
            {
                return false;
            }

            CodePoint = (CodePoint << 4) | static_cast<unsigned int>(Value);
        }

        return true;
    }

    void AppendUtf8(std::string& Text, unsigned int CodePoint)
    {
        if (CodePoint <= 0x7F)
        {
            Text.push_back(static_cast<char>(CodePoint));
            return;
        }

        if (CodePoint <= 0x7FF)
        {
            Text.push_back(static_cast<char>(0xC0 | (CodePoint >> 6)));
            Text.push_back(static_cast<char>(0x80 | (CodePoint & 0x3F)));
            return;
        }

        if (CodePoint <= 0xFFFF)
        {
            Text.push_back(static_cast<char>(0xE0 | (CodePoint >> 12)));
            Text.push_back(static_cast<char>(0x80 | ((CodePoint >> 6) & 0x3F)));
            Text.push_back(static_cast<char>(0x80 | (CodePoint & 0x3F)));
            return;
        }

        Text.push_back(static_cast<char>(0xF0 | (CodePoint >> 18)));
        Text.push_back(static_cast<char>(0x80 | ((CodePoint >> 12) & 0x3F)));
        Text.push_back(static_cast<char>(0x80 | ((CodePoint >> 6) & 0x3F)));
        Text.push_back(static_cast<char>(0x80 | (CodePoint & 0x3F)));
    }

    std::string EscapeJsonString(const std::string& Text)
    {
        std::ostringstream Stream;

        for (char Character : Text)
        {
            switch (Character)
            {
            case '\\':
                Stream << "\\\\";
                break;
            case '"':
                Stream << "\\\"";
                break;
            case '\n':
                Stream << "\\n";
                break;
            case '\r':
                Stream << "\\r";
                break;
            case '\t':
                Stream << "\\t";
                break;
            default:
                Stream << Character;
                break;
            }
        }

        return Stream.str();
    }

    std::string UnescapeJsonString(const std::string& Text)
    {
        std::ostringstream Stream;

        for (std::size_t Index = 0; Index < Text.size(); ++Index)
        {
            char Character = Text[Index];
            if (Character != '\\' || Index + 1 >= Text.size())
            {
                Stream << Character;
                continue;
            }

            char Escaped = Text[++Index];
            switch (Escaped)
            {
            case 'n':
                Stream << '\n';
                break;
            case 'r':
                Stream << '\r';
                break;
            case 't':
                Stream << '\t';
                break;
            case '"':
                Stream << '"';
                break;
            case '\\':
                Stream << '\\';
                break;
            case '/':
                Stream << '/';
                break;
            case 'u':
            {
                unsigned int CodePoint = 0;
                if (!TryReadUnicodeEscape(Text, Index + 1, CodePoint))
                {
                    Stream << 'u';
                    break;
                }

                Index += 4;

                if (CodePoint >= 0xD800 && CodePoint <= 0xDBFF)
                {
                    if (Index + 6 < Text.size() && Text[Index + 1] == '\\' && Text[Index + 2] == 'u')
                    {
                        unsigned int LowSurrogate = 0;
                        if (TryReadUnicodeEscape(Text, Index + 3, LowSurrogate)
                            && LowSurrogate >= 0xDC00
                            && LowSurrogate <= 0xDFFF)
                        {
                            CodePoint = 0x10000
                                + ((CodePoint - 0xD800) << 10)
                                + (LowSurrogate - 0xDC00);
                            Index += 6;
                        }
                    }
                }

                std::string Utf8Text;
                AppendUtf8(Utf8Text, CodePoint);
                Stream << Utf8Text;
                break;
            }
            default:
                Stream << Escaped;
                break;
            }
        }

        return Stream.str();
    }

#ifdef _WIN32
    class FWinSockSession
    {
    public:
        FWinSockSession()
        {
            WSADATA Data;
            if (WSAStartup(MAKEWORD(2, 2), &Data) != 0)
            {
                throw std::runtime_error("WSAStartup failed.");
            }
        }

        ~FWinSockSession()
        {
            WSACleanup();
        }
    };
#endif
}

FOllamaClient::FOllamaClient(std::string InHost, int InPort, std::string InModel)
    : Host(std::move(InHost))
    , Port(InPort)
    , Model(std::move(InModel))
{
}

std::string FOllamaClient::Generate(const std::string& Prompt) const
{
    const std::string Body = BuildRequestBody(Prompt);
    const std::string HttpResponse = SendHttpPost("/api/generate", Body);
    return ExtractResponseText(HttpResponse);
}

const std::string& FOllamaClient::GetModel() const
{
    return Model;
}

std::string FOllamaClient::BuildRequestBody(const std::string& Prompt) const
{
    std::ostringstream Body;
    Body << "{"
         << "\"model\":\"" << EscapeJsonString(Model) << "\","
         << "\"prompt\":\"" << EscapeJsonString(Prompt) << "\","
         << "\"stream\":false"
         << "}";
    return Body.str();
}

std::string FOllamaClient::SendHttpPost(const std::string& Path, const std::string& Body) const
{
#ifndef _WIN32
    throw std::runtime_error("This learning sample currently implements the Ollama HTTP client for Windows only.");
#else
    FWinSockSession Session;

    addrinfo Hints{};
    Hints.ai_family = AF_INET;
    Hints.ai_socktype = SOCK_STREAM;
    Hints.ai_protocol = IPPROTO_TCP;

    addrinfo* Result = nullptr;
    const std::string PortText = std::to_string(Port);
    if (getaddrinfo(Host.c_str(), PortText.c_str(), &Hints, &Result) != 0)
    {
        throw std::runtime_error("Could not resolve Ollama host.");
    }

    SOCKET Socket = socket(Result->ai_family, Result->ai_socktype, Result->ai_protocol);
    if (Socket == INVALID_SOCKET)
    {
        freeaddrinfo(Result);
        throw std::runtime_error("Could not create socket.");
    }

    if (connect(Socket, Result->ai_addr, static_cast<int>(Result->ai_addrlen)) == SOCKET_ERROR)
    {
        freeaddrinfo(Result);
        closesocket(Socket);
        throw std::runtime_error("Could not connect to Ollama. Make sure Ollama is running on localhost:11434.");
    }

    freeaddrinfo(Result);

    std::ostringstream Request;
    Request << "POST " << Path << " HTTP/1.1\r\n"
            << "Host: " << Host << ":" << Port << "\r\n"
            << "Content-Type: application/json\r\n"
            << "Content-Length: " << Body.size() << "\r\n"
            << "Connection: close\r\n"
            << "\r\n"
            << Body;

    const std::string RequestText = Request.str();
    int Sent = send(Socket, RequestText.c_str(), static_cast<int>(RequestText.size()), 0);
    if (Sent == SOCKET_ERROR)
    {
        closesocket(Socket);
        throw std::runtime_error("Could not send request to Ollama.");
    }

    std::string Response;
    char Buffer[4096];
    int Received = 0;
    while ((Received = recv(Socket, Buffer, sizeof(Buffer), 0)) > 0)
    {
        Response.append(Buffer, Received);
    }

    closesocket(Socket);
    return Response;
#endif
}

std::string FOllamaClient::ExtractResponseText(const std::string& HttpResponse) const
{
    const std::string HeaderSeparator = "\r\n\r\n";
    const std::size_t BodyStart = HttpResponse.find(HeaderSeparator);
    if (BodyStart == std::string::npos)
    {
        throw std::runtime_error("Ollama returned an invalid HTTP response.");
    }

    const std::string Body = HttpResponse.substr(BodyStart + HeaderSeparator.size());
    const std::string Key = "\"response\":\"";
    const std::size_t ResponseStart = Body.find(Key);
    if (ResponseStart == std::string::npos)
    {
        throw std::runtime_error("Ollama response did not contain a response field.");
    }

    std::size_t ValueStart = ResponseStart + Key.size();
    std::string EncodedText;
    bool bIsEscaped = false;

    for (std::size_t Index = ValueStart; Index < Body.size(); ++Index)
    {
        char Character = Body[Index];
        if (bIsEscaped)
        {
            EncodedText.push_back('\\');
            EncodedText.push_back(Character);
            bIsEscaped = false;
            continue;
        }

        if (Character == '\\')
        {
            bIsEscaped = true;
            continue;
        }

        if (Character == '"')
        {
            return UnescapeJsonString(EncodedText);
        }

        EncodedText.push_back(Character);
    }

    throw std::runtime_error("Ollama response field was not closed.");
}
