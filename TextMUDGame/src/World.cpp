#include "World.h"

#include <stdexcept>
#include <utility>

World::World(std::map<std::string, Room> rooms)
    : rooms_(std::move(rooms))
{
}

World World::createDefaultWorld()
{
    Item oldKey(
        "old_key",
        "낡은 열쇠",
        "성문을 열 수 있을 것 같은 오래된 열쇠입니다.");

    NPC merchant(
        "merchant",
        "상인",
        "상인: 필요한 물건이 있다면 주변을 잘 살펴보게.",
        true,
        "친절하지만 장사꾼답게 약간 계산적이다.",
        "- 잡화점에는 오래된 물건이 많다.\n"
        "- 낡은 열쇠는 성문과 관련이 있을 수 있다.\n"
        "- 마을 사람들은 성 안쪽을 조심스러워한다.");

    NPC guard(
        "guard",
        "경비병",
        "경비병: 성문은 아무나 통과할 수 없다. 오래된 열쇠가 있다면 이야기가 달라질지도 모르지.",
        true,
        "신중하고 규칙을 중요하게 생각한다.",
        "- 성문은 낡은 열쇠가 있어야 열 수 있다.\n"
        "- 성문 너머에는 오래된 성 안뜰이 있다.\n"
        "- 성문은 함부로 열 수 없다.");

    Room villageSquare(
        "village_square",
        "마을 광장",
        "작은 마을의 중심지입니다. 동쪽에는 잡화점이 있고, 남쪽에는 성문으로 가는 길이 있습니다.",
        {{"east", "general_store"}, {"south", "castle_gate"}},
        {},
        {});

    Room generalStore(
        "general_store",
        "잡화점",
        "오래된 나무 선반 위에 여러 물건이 놓여 있습니다.",
        {{"west", "village_square"}},
        {oldKey},
        {merchant});

    Room castleGate(
        "castle_gate",
        "성문 앞",
        "거대한 성문이 굳게 닫혀 있습니다. 누군가 이곳을 지키고 있습니다.",
        {{"north", "village_square"}, {"south", "castle_courtyard"}},
        {},
        {guard});

    Room castleCourtyard(
        "castle_courtyard",
        "성 안뜰",
        "오래된 성 안뜰입니다. 낡은 돌바닥과 무너진 분수가 보입니다.",
        {{"north", "castle_gate"}},
        {},
        {});

    return World({
        {villageSquare.getId(), villageSquare},
        {generalStore.getId(), generalStore},
        {castleGate.getId(), castleGate},
        {castleCourtyard.getId(), castleCourtyard}
    });
}

Room& World::getRoom(const std::string& roomId)
{
    const auto it = rooms_.find(roomId);
    if (it == rooms_.end())
    {
        throw std::out_of_range("Room not found: " + roomId);
    }

    return it->second;
}

const Room& World::getRoom(const std::string& roomId) const
{
    const auto it = rooms_.find(roomId);
    if (it == rooms_.end())
    {
        throw std::out_of_range("Room not found: " + roomId);
    }

    return it->second;
}

bool World::hasRoom(const std::string& roomId) const
{
    return rooms_.find(roomId) != rooms_.end();
}

void World::removeItemFromAllRooms(const std::string& itemId)
{
    for (auto& roomEntry : rooms_)
    {
        roomEntry.second.removeItem(itemId);
    }
}

void World::removeInventoryItemsFromRooms(const std::vector<Item>& inventory)
{
    for (const Item& item : inventory)
    {
        removeItemFromAllRooms(item.getId());
    }
}
