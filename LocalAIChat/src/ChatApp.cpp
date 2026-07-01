#include "ChatApp.h"

#include <iostream>
#include <stdexcept>
#include <utility>

#ifdef _WIN32
#include <windows.h>
#include <tlhelp32.h>
#endif

FChatApp::FChatApp(std::unique_ptr<FAIEngine> InAIEngine)
    : AIEngine(std::move(InAIEngine))
{
}

void FChatApp::Run()
{
    PrintWelcome();

    bool bShouldExit = false;
    while (!bShouldExit)
    {
        std::cout << "\nUser: ";

        std::string Input;
        if (!std::getline(std::cin, Input))
        {
            break;
        }

        if (Input.empty())
        {
            continue;
        }

        if (HandleCommand(Input, bShouldExit))
        {
            continue;
        }

        AddMessage(EMessageRole::User, Input);

        try
        {
            std::cout << "AI 답변 생성 중...\n";
            const std::string Response = AIEngine->GenerateResponse(Input);
            AddMessage(EMessageRole::Assistant, Response);
            std::cout << "AI: " << Response << '\n';
        }
        catch (const std::exception& Error)
        {
            std::cout << "AI 오류: " << Error.what() << '\n';
        }
    }

    std::cout << "Local AI Chat을 종료합니다.\n";
    ShutdownOllamaServer();
}

void FChatApp::PrintWelcome() const
{
    std::cout << "Local AI Chat Start!\n";
    std::cout << "Ollama model: qwen3:8b\n";
    std::cout << "명령어는 /help 를 입력해서 확인할 수 있습니다.\n";
}

void FChatApp::PrintHelp() const
{
    std::cout << "\n사용 가능한 명령어\n";
    std::cout << "/help    명령어 보기\n";
    std::cout << "/history 대화 기록 보기\n";
    std::cout << "/clear   대화 기록 삭제\n";
    std::cout << "/exit    프로그램 종료\n";
}

void FChatApp::PrintHistory() const
{
    if (Messages.empty())
    {
        std::cout << "대화 기록이 비어 있습니다.\n";
        return;
    }

    for (const FChatMessage& Message : Messages)
    {
        std::cout << Message.GetRoleName() << ": " << Message.GetText() << '\n';
    }
}

void FChatApp::ShutdownOllamaServer() const
{
#ifndef _WIN32
    std::cout << "Ollama 서버 자동 종료는 Windows 실행 환경에서만 지원합니다.\n";
#else
    HANDLE Snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (Snapshot == INVALID_HANDLE_VALUE)
    {
        std::cout << "Ollama 서버 종료 상태를 확인하지 못했습니다.\n";
        return;
    }

    PROCESSENTRY32W ProcessEntry{};
    ProcessEntry.dwSize = sizeof(ProcessEntry);

    bool bFoundOllama = false;
    bool bStoppedOllama = false;

    if (Process32FirstW(Snapshot, &ProcessEntry))
    {
        do
        {
            if (std::wstring(ProcessEntry.szExeFile) != L"ollama.exe")
            {
                continue;
            }

            bFoundOllama = true;
            HANDLE Process = OpenProcess(PROCESS_TERMINATE, FALSE, ProcessEntry.th32ProcessID);
            if (Process == nullptr)
            {
                continue;
            }

            if (TerminateProcess(Process, 0) != 0)
            {
                bStoppedOllama = true;
            }

            CloseHandle(Process);
        } while (Process32NextW(Snapshot, &ProcessEntry));
    }

    CloseHandle(Snapshot);

    if (bStoppedOllama)
    {
        std::cout << "Ollama 서버를 종료했습니다.\n";
        return;
    }

    if (bFoundOllama)
    {
        std::cout << "Ollama 서버 프로세스를 찾았지만 종료하지 못했습니다.\n";
        return;
    }

    std::cout << "실행 중인 Ollama 서버가 없습니다.\n";
#endif
}

bool FChatApp::HandleCommand(const std::string& Input, bool& bShouldExit)
{
    if (Input == "exit" || Input == "/exit")
    {
        bShouldExit = true;
        return true;
    }

    if (Input == "/help")
    {
        PrintHelp();
        return true;
    }

    if (Input == "/history")
    {
        PrintHistory();
        return true;
    }

    if (Input == "/clear")
    {
        Messages.clear();
        std::cout << "대화 기록을 삭제했습니다.\n";
        return true;
    }

    if (!Input.empty() && Input[0] == '/')
    {
        std::cout << "알 수 없는 명령어입니다. /help 를 입력해보세요.\n";
        return true;
    }

    return false;
}

void FChatApp::AddMessage(EMessageRole Role, const std::string& Text)
{
    Messages.emplace_back(Role, Text);
}
