#pragma once

#include "OllamaClient.h"

#include <string>

class FAIEngine
{
public:
    explicit FAIEngine(FOllamaClient InOllamaClient);

    std::string GenerateResponse(const std::string& UserInput) const;

private:
    std::string BuildPrompt(const std::string& UserInput) const;

    FOllamaClient OllamaClient;
};
