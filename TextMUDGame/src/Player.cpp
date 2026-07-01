#include "Player.h"

#include <utility>

Player::Player(std::string startRoomId)
    : currentRoomId_(std::move(startRoomId))
{
}

const std::string& Player::getCurrentRoomId() const { return currentRoomId_; }

void Player::setCurrentRoomId(const std::string& roomId)
{
    currentRoomId_ = roomId;
}

void Player::addItem(const Item& item)
{
    inventory_.push_back(item);
}

bool Player::removeItem(const std::string& itemId)
{
    for (auto it = inventory_.begin(); it != inventory_.end(); ++it)
    {
        if (it->getId() == itemId)
        {
            inventory_.erase(it);
            return true;
        }
    }

    return false;
}

void Player::setInventory(const std::vector<Item>& items)
{
    inventory_ = items;
}

bool Player::hasItem(const std::string& itemId) const
{
    for (const Item& item : inventory_)
    {
        if (item.getId() == itemId)
        {
            return true;
        }
    }

    return false;
}

const std::vector<Item>& Player::getInventory() const { return inventory_; }
bool Player::isFirstQuestCompleted() const { return firstQuestCompleted_; }
void Player::completeFirstQuest() { firstQuestCompleted_ = true; }
