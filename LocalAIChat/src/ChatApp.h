#pragma once

#include "AIEngine.h"
#include "ChatMessage.h"

#include <memory>
#include <string>
#include <vector>

class FChatApp
{
public:
    explicit FChatApp(std::unique_ptr<FAIEngine> InAIEngine);

    void Run();

private:
    void PrintWelcome() const;
    void PrintHelp() const;
    void PrintHistory() const;
    void ShutdownOllamaServer() const;
    bool HandleCommand(const std::string& Input, bool& bShouldExit);
    void AddMessage(EMessageRole Role, const std::string& Text);

    std::unique_ptr<FAIEngine> AIEngine;
    std::vector<FChatMessage> Messages;
};
