#pragma once

#include "Item.h"
#include "NPC.h"

#include <map>
#include <optional>
#include <string>
#include <vector>

class Room
{
public:
    Room() = default;
    Room(std::string id,
         std::string name,
         std::string description,
         std::map<std::string, std::string> exits,
         std::vector<Item> items,
         std::vector<NPC> npcs);

    const std::string& getId() const;
    const std::string& getName() const;
    const std::string& getDescription() const;
    const std::map<std::string, std::string>& getExits() const;
    const std::vector<Item>& getItems() const;
    const std::vector<NPC>& getNpcs() const;

    std::optional<std::string> getExitRoomId(const std::string& direction) const;
    std::optional<Item> takeItem(const std::string& itemId);
    void removeItem(const std::string& itemId);
    const NPC* findNpc(const std::string& npcId) const;

private:
    std::string id_;
    std::string name_;
    std::string description_;
    std::map<std::string, std::string> exits_;
    std::vector<Item> items_;
    std::vector<NPC> npcs_;
};
