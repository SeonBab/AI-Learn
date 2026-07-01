#pragma once

#include <string>

class FOllamaClient
{
public:
    FOllamaClient(std::string InHost, int InPort, std::string InModel);

    std::string Generate(const std::string& Prompt) const;
    const std::string& GetModel() const;

private:
    std::string BuildRequestBody(const std::string& Prompt) const;
    std::string SendHttpPost(const std::string& Path, const std::string& Body) const;
    std::string ExtractResponseText(const std::string& HttpResponse) const;

    std::string Host;
    int Port;
    std::string Model;
};
