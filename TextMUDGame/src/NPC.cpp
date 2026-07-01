#include "NPC.h"

#include <utility>

NPC::NPC(std::string id,
         std::string name,
         std::string fixedDialogue,
         bool aiEnabled,
         std::string personality,
         std::string knowledge)
    : id_(std::move(id)),
      name_(std::move(name)),
      fixedDialogue_(std::move(fixedDialogue)),
      aiEnabled_(aiEnabled),
      personality_(std::move(personality)),
      knowledge_(std::move(knowledge))
{
}

const std::string& NPC::getId() const
{
    return id_;
}

const std::string& NPC::getName() const
{
    return name_;
}

const std::string& NPC::getFixedDialogue() const
{
    return fixedDialogue_;
}

bool NPC::isAiEnabled() const
{
    return aiEnabled_;
}

const std::string& NPC::getPersonality() const
{
    return personality_;
}

const std::string& NPC::getKnowledge() const
{
    return knowledge_;
}
