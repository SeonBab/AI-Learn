#include "SaveManager.h"

#include <fstream>
#include <map>
#include <sstream>

bool SaveManager::save(const Player& player, const GameState& gameState, const std::string& filePath) const
{
    std::ofstream file(filePath);
    if (!file.is_open())
    {
        return false;
    }

    file << "currentRoomId=" << player.getCurrentRoomId() << "\n";
    file << "inventory=" << joinInventory(player.getInventory()) << "\n";
    file << "castleGateUnlocked=" << (gameState.isCastleGateUnlocked() ? "true" : "false") << "\n";
    file << "reachedCourtyard=" << (gameState.hasReachedCourtyard() ? "true" : "false") << "\n";

    return file.good();
}

bool SaveManager::load(Player& player, GameState& gameState, const std::string& filePath) const
{
    std::ifstream file(filePath);
    if (!file.is_open())
    {
        return false;
    }

    std::map<std::string, std::string> values;
    std::string line;

    while (std::getline(file, line))
    {
        const std::size_t separator = line.find('=');
        if (separator == std::string::npos)
        {
            continue;
        }

        const std::string key = line.substr(0, separator);
        const std::string value = line.substr(separator + 1);
        values[key] = value;
    }

    const auto roomIt = values.find("currentRoomId");
    if (roomIt == values.end() || roomIt->second.empty())
    {
        return false;
    }

    player.setCurrentRoomId(roomIt->second);
    player.setInventory(splitInventory(values["inventory"]));
    gameState.setCastleGateUnlocked(parseBool(values["castleGateUnlocked"]));
    gameState.setReachedCourtyard(parseBool(values["reachedCourtyard"]));

    return true;
}

std::string SaveManager::joinInventory(const std::vector<Item>& inventory) const
{
    std::ostringstream output;

    for (std::size_t i = 0; i < inventory.size(); ++i)
    {
        if (i > 0)
        {
            output << ",";
        }

        output << inventory[i].getId();
    }

    return output.str();
}

std::vector<Item> SaveManager::splitInventory(const std::string& text) const
{
    std::vector<Item> inventory;
    std::istringstream stream(text);
    std::string itemId;

    while (std::getline(stream, itemId, ','))
    {
        if (!itemId.empty())
        {
            inventory.push_back(createItemById(itemId));
        }
    }

    return inventory;
}

bool SaveManager::parseBool(const std::string& text) const
{
    return text == "true";
}

Item SaveManager::createItemById(const std::string& itemId) const
{
    if (itemId == "old_key")
    {
        return Item("old_key", "낡은 열쇠", "성문을 열 수 있을 것 같은 오래된 열쇠입니다.");
    }

    return Item(itemId, itemId, "저장 파일에서 불러온 아이템입니다.");
}
