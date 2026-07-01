#pragma once

#include "CommandParser.h"
#include "GameState.h"
#include "LLMClient.h"
#include "Player.h"
#include "SaveManager.h"
#include "World.h"

#include <string>

class GameEngine
{
public:
    GameEngine();
    ~GameEngine();

    std::string getIntroText() const;
    std::string processCommand(const std::string& inputText);
    bool isRunning() const;

private:
    std::string handleLook() const;
    std::string handleMove(const Command& command);
    std::string handleNpc() const;
    std::string handleItems() const;
    std::string handleTake(const Command& command);
    std::string handleUse(const Command& command);
    std::string handleInventory() const;
    std::string handleTalk(const Command& command) const;
    std::string handleAsk(const Command& command) const;
    std::string handleSave() const;
    std::string handleLoad();
    std::string getHelpText() const;

    std::string buildNPCPrompt(const NPC& npc, const std::string& question) const;
    std::string formatInventoryForPrompt() const;
    std::string formatRoom(const Room& room) const;

    World world_;
    Player player_;
    GameState state_;
    SaveManager saveManager_;
    LLMClient llmClient_;
    CommandParser parser_;
    std::string aiStatusMessage_;
    std::string saveFilePath_ = "save.txt";
    bool running_ = true;
};
