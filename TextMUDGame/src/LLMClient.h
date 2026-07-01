#pragma once

#include <cstddef>
#include <string>

class LLMClient
{
public:
    LLMClient();
    LLMClient(std::string host, int port, std::string modelName, bool mockMode);

    bool initialize();
    std::string generateResponse(const std::string& prompt) const;
    std::string generate(const std::string& prompt) const;
    void shutdown();

    bool isAvailable() const;
    bool isMockMode() const;
    const std::string& getModelName() const;

private:
    std::string generateMockResponse(const std::string& prompt) const;
    std::string generateOllamaResponse(const std::string& prompt) const;
    std::string postGenerate(const std::string& prompt, const std::string& keepAliveValue) const;
    std::string buildRequestBody(const std::string& prompt, const std::string& keepAliveValue) const;
    std::string sendHttpPost(const std::string& path, const std::string& body) const;
    std::string extractResponseText(const std::string& httpResponse) const;
    std::string removeThinkTags(const std::string& text) const;
    std::string trimResponseLength(const std::string& text) const;
    std::string trimWhitespace(const std::string& text) const;

    bool mockMode_ = false;
    bool available_ = false;
    std::string host_ = "localhost";
    int port_ = 11434;
    std::string modelName_ = "qwen3:8b";
    std::string keepAlive_ = "10m";
    int connectionTimeoutSeconds_ = 5;
    int readTimeoutSeconds_ = 60;
    int writeTimeoutSeconds_ = 5;
    std::size_t maxResponseLength_ = 500;
};
