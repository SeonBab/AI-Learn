#include "Room.h"

#include <utility>

Room::Room(std::string id,
           std::string name,
           std::string description,
           std::map<std::string, std::string> exits,
           std::vector<Item> items,
           std::vector<NPC> npcs)
    : id_(std::move(id)),
      name_(std::move(name)),
      description_(std::move(description)),
      exits_(std::move(exits)),
      items_(std::move(items)),
      npcs_(std::move(npcs))
{
}

const std::string& Room::getId() const { return id_; }
const std::string& Room::getName() const { return name_; }
const std::string& Room::getDescription() const { return description_; }
const std::map<std::string, std::string>& Room::getExits() const { return exits_; }
const std::vector<Item>& Room::getItems() const { return items_; }
const std::vector<NPC>& Room::getNpcs() const { return npcs_; }

std::optional<std::string> Room::getExitRoomId(const std::string& direction) const
{
    const auto it = exits_.find(direction);
    if (it == exits_.end())
    {
        return std::nullopt;
    }

    return it->second;
}

std::optional<Item> Room::takeItem(const std::string& itemId)
{
    for (auto it = items_.begin(); it != items_.end(); ++it)
    {
        if (it->getId() == itemId)
        {
            Item item = *it;
            items_.erase(it);
            return item;
        }
    }

    return std::nullopt;
}

void Room::removeItem(const std::string& itemId)
{
    for (auto it = items_.begin(); it != items_.end();)
    {
        if (it->getId() == itemId)
        {
            it = items_.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

const NPC* Room::findNpc(const std::string& npcId) const
{
    for (const NPC& npc : npcs_)
    {
        if (npc.getId() == npcId)
        {
            return &npc;
        }
    }

    return nullptr;
}
