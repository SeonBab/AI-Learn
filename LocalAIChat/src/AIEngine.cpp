#include "AIEngine.h"

#include <algorithm>
#include <cctype>
#include <sstream>
#include <utility>

namespace
{
    std::string TrimAsciiWhitespace(const std::string& Text)
    {
        const auto First = std::find_if_not(Text.begin(), Text.end(), [](unsigned char Character)
        {
            return std::isspace(Character) != 0;
        });

        const auto Last = std::find_if_not(Text.rbegin(), Text.rend(), [](unsigned char Character)
        {
            return std::isspace(Character) != 0;
        }).base();

        if (First >= Last)
        {
            return "";
        }

        return std::string(First, Last);
    }

    std::string ToLowerAscii(std::string Text)
    {
        std::transform(Text.begin(), Text.end(), Text.begin(), [](unsigned char Character)
        {
            return static_cast<char>(std::tolower(Character));
        });

        return Text;
    }

    bool IsSimpleGreeting(const std::string& UserInput)
    {
        const std::string Text = ToLowerAscii(TrimAsciiWhitespace(UserInput));
        return Text == "안녕"
            || Text == "안녕하세요"
            || Text == "하이"
            || Text == "hi"
            || Text == "hello";
    }
}

FAIEngine::FAIEngine(FOllamaClient InOllamaClient)
    : OllamaClient(std::move(InOllamaClient))
{
}

std::string FAIEngine::GenerateResponse(const std::string& UserInput) const
{
    if (IsSimpleGreeting(UserInput))
    {
        return "안녕하세요. 무엇을 도와드릴까요?";
    }

    return OllamaClient.Generate(BuildPrompt(UserInput));
}

std::string FAIEngine::BuildPrompt(const std::string& UserInput) const
{
    std::ostringstream Prompt;
    Prompt << "너는 C++ 학습을 돕는 로컬 AI 채팅 assistant다.\n"
           << "사용자의 의도를 먼저 판단해서 필요한 만큼만 답변한다.\n"
           << "간단한 인사, 잡담, 확인 질문에는 한 문장으로만 짧고 자연스럽게 답한다.\n"
           << "간단한 인사에는 예시, 목록, 코드, 학습 안내를 붙이지 않는다.\n"
           << "사용자 질문이 단순 인사라면 정확히 '안녕하세요.'라고만 답한다.\n"
           << "C++ 개념 설명, 코드 작성, 오류 해결, 학습 정리 요청일 때만 학습 노트 형식의 Markdown 답변을 사용한다.\n"
           << "학습 노트 형식이 필요할 때는 '## 설명'과 '## 핵심 정리' 섹션을 포함한다.\n"
           << "C++ 코드가 필요할 때만 cpp 코드 블록을 사용한다.\n"
           << "사용자가 요청하지 않은 예제 코드, 긴 목록, 과한 자기소개는 피한다.\n\n"
           << "사용자 질문:\n"
           << UserInput;
    return Prompt.str();
}
