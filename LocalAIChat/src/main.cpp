#include "AIEngine.h"
#include "ChatApp.h"
#include "OllamaClient.h"

#include <cstdlib>
#include <chrono>
#include <iostream>
#include <memory>
#include <string>
#include <thread>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#endif

namespace
{
#ifdef _WIN32
    class FWinSockSession
    {
    public:
        FWinSockSession()
        {
            WSADATA Data;
            bIsStarted = WSAStartup(MAKEWORD(2, 2), &Data) == 0;
        }

        ~FWinSockSession()
        {
            if (bIsStarted)
            {
                WSACleanup();
            }
        }

        bool IsStarted() const
        {
            return bIsStarted;
        }

    private:
        bool bIsStarted = false;
    };
#endif

    void ConfigureConsoleEncoding()
    {
#ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);
#endif
    }

    std::wstring Utf8ToWide(const std::string& Text)
    {
#ifdef _WIN32
        if (Text.empty())
        {
            return L"";
        }

        const int Size = MultiByteToWideChar(CP_UTF8, 0, Text.c_str(), -1, nullptr, 0);
        if (Size <= 0)
        {
            return L"";
        }

        std::wstring Result(static_cast<std::size_t>(Size - 1), L'\0');
        MultiByteToWideChar(CP_UTF8, 0, Text.c_str(), -1, Result.data(), Size);
        return Result;
#else
        return L"";
#endif
    }

    std::string GetEnvironmentOrDefault(const char* Name, const std::string& DefaultValue)
    {
#ifdef _MSC_VER
        char* Value = nullptr;
        std::size_t ValueLength = 0;
        if (_dupenv_s(&Value, &ValueLength, Name) != 0 || Value == nullptr || ValueLength == 0)
        {
            std::free(Value);
            return DefaultValue;
        }

        std::string Result(Value);
        std::free(Value);
        return Result;
#else
        const char* Value = std::getenv(Name);
        if (Value == nullptr || std::string(Value).empty())
        {
            return DefaultValue;
        }

        return Value;
#endif
    }

    bool IsOllamaServerAvailable()
    {
#ifndef _WIN32
        return true;
#else
        FWinSockSession Session;
        if (!Session.IsStarted())
        {
            return false;
        }

        addrinfo Hints{};
        Hints.ai_family = AF_INET;
        Hints.ai_socktype = SOCK_STREAM;
        Hints.ai_protocol = IPPROTO_TCP;

        addrinfo* Result = nullptr;
        if (getaddrinfo("127.0.0.1", "11434", &Hints, &Result) != 0)
        {
            return false;
        }

        SOCKET Socket = socket(Result->ai_family, Result->ai_socktype, Result->ai_protocol);
        if (Socket == INVALID_SOCKET)
        {
            freeaddrinfo(Result);
            return false;
        }

        const bool bIsConnected = connect(Socket, Result->ai_addr, static_cast<int>(Result->ai_addrlen)) != SOCKET_ERROR;
        freeaddrinfo(Result);
        closesocket(Socket);
        return bIsConnected;
#endif
    }

    bool StartOllamaServer()
    {
#ifndef _WIN32
        return true;
#else
        const std::string LocalAppData = GetEnvironmentOrDefault("LOCALAPPDATA", "");
        if (LocalAppData.empty())
        {
            return false;
        }

        const std::wstring OllamaPath = Utf8ToWide(LocalAppData + "\\Programs\\Ollama\\ollama.exe");
        if (OllamaPath.empty() || GetFileAttributesW(OllamaPath.c_str()) == INVALID_FILE_ATTRIBUTES)
        {
            return false;
        }

        std::wstring CommandLine = L"\"" + OllamaPath + L"\" serve";
        STARTUPINFOW StartupInfo{};
        StartupInfo.cb = sizeof(StartupInfo);
        StartupInfo.dwFlags = STARTF_USESHOWWINDOW;
        StartupInfo.wShowWindow = SW_HIDE;

        PROCESS_INFORMATION ProcessInfo{};
        const BOOL bStarted = CreateProcessW(
            nullptr,
            CommandLine.data(),
            nullptr,
            nullptr,
            FALSE,
            CREATE_NO_WINDOW,
            nullptr,
            nullptr,
            &StartupInfo,
            &ProcessInfo);

        if (bStarted == 0)
        {
            return false;
        }

        CloseHandle(ProcessInfo.hThread);
        CloseHandle(ProcessInfo.hProcess);
        return true;
#endif
    }

    bool EnsureOllamaServer()
    {
        if (IsOllamaServerAvailable())
        {
            return true;
        }

        std::cout << "Ollama 서버를 시작합니다.\n";
        if (!StartOllamaServer())
        {
            std::cout << "Ollama 서버를 자동으로 시작하지 못했습니다.\n";
            return false;
        }

        for (int Attempt = 0; Attempt < 30; ++Attempt)
        {
            if (IsOllamaServerAvailable())
            {
                std::cout << "Ollama 서버가 준비되었습니다.\n";
                return true;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }

        std::cout << "Ollama 서버가 제한 시간 안에 준비되지 않았습니다.\n";
        return false;
    }
}

int main()
{
    ConfigureConsoleEncoding();
    EnsureOllamaServer();

    const std::string Model = GetEnvironmentOrDefault("OLLAMA_MODEL", "qwen3:8b");
    FOllamaClient OllamaClient("127.0.0.1", 11434, Model);
    auto AIEngine = std::make_unique<FAIEngine>(OllamaClient);

    FChatApp App(std::move(AIEngine));
    App.Run();

    return 0;
}
