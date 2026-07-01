#pragma once

#include "GameState.h"
#include "Player.h"

#include <string>
#include <vector>

class SaveManager
{
public:
    bool save(const Player& player, const GameState& gameState, const std::string& filePath) const;
    bool load(Player& player, GameState& gameState, const std::string& filePath) const;

private:
    std::string joinInventory(const std::vector<Item>& inventory) const;
    std::vector<Item> splitInventory(const std::string& text) const;
    bool parseBool(const std::string& text) const;
    Item createItemById(const std::string& itemId) const;
};
