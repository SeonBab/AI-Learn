#pragma once

#include "Room.h"

#include <map>
#include <string>
#include <vector>

class World
{
public:
    static World createDefaultWorld();

    Room& getRoom(const std::string& roomId);
    const Room& getRoom(const std::string& roomId) const;
    bool hasRoom(const std::string& roomId) const;
    void removeItemFromAllRooms(const std::string& itemId);
    void removeInventoryItemsFromRooms(const std::vector<Item>& inventory);

private:
    explicit World(std::map<std::string, Room> rooms);

    std::map<std::string, Room> rooms_;
};
