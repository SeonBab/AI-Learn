#pragma once

#include <string>

class NPC
{
public:
    NPC() = default;
    NPC(std::string id,
        std::string name,
        std::string fixedDialogue,
        bool aiEnabled = false,
        std::string personality = "",
        std::string knowledge = "");

    const std::string& getId() const;
    const std::string& getName() const;
    const std::string& getFixedDialogue() const;
    bool isAiEnabled() const;
    const std::string& getPersonality() const;
    const std::string& getKnowledge() const;

private:
    std::string id_;
    std::string name_;
    std::string fixedDialogue_;
    bool aiEnabled_ = false;
    std::string personality_;
    std::string knowledge_;
};
