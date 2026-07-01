#pragma once

#include "Item.h"

#include <string>
#include <vector>

class Player
{
public:
    explicit Player(std::string startRoomId = "village_square");

    const std::string& getCurrentRoomId() const;
    void setCurrentRoomId(const std::string& roomId);

    void addItem(const Item& item);
    bool removeItem(const std::string& itemId);
    void setInventory(const std::vector<Item>& items);
    bool hasItem(const std::string& itemId) const;
    const std::vector<Item>& getInventory() const;

    bool isFirstQuestCompleted() const;
    void completeFirstQuest();

private:
    std::string currentRoomId_;
    std::vector<Item> inventory_;
    bool firstQuestCompleted_ = false;
};
