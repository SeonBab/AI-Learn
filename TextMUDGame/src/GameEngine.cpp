#include "GameEngine.h"

#include <optional>
#include <sstream>

GameEngine::GameEngine()
    : world_(World::createDefaultWorld()),
      player_("village_square")
{
    if (llmClient_.initialize())
    {
        aiStatusMessage_ = "[알림] AI NPC 기능이 활성화되었습니다.\n";
    }
    else
    {
        aiStatusMessage_ = "[알림] 로컬 LLM을 사용할 수 없어 AI NPC 질문 기능이 비활성화됩니다.\n";
    }
}

GameEngine::~GameEngine()
{
    llmClient_.shutdown();
}

std::string GameEngine::getIntroText() const
{
    std::ostringstream output;
    output << "C++ 로컬 AI 텍스트 MUD 게임\n";
    output << "5차 시나리오: 낡은 성문의 비밀\n";
    output << aiStatusMessage_;
    output << "'help'를 입력하면 명령어를 볼 수 있고, 'quit'을 입력하면 게임을 종료합니다.\n";
    output << formatRoom(world_.getRoom(player_.getCurrentRoomId()));
    return output.str();
}

std::string GameEngine::processCommand(const std::string& inputText)
{
    const Command command = parser_.parse(inputText);

    switch (command.type)
    {
    case CommandType::Look:
        return handleLook();
    case CommandType::Move:
        return handleMove(command);
    case CommandType::Npc:
        return handleNpc();
    case CommandType::Items:
        return handleItems();
    case CommandType::Take:
        return handleTake(command);
    case CommandType::Use:
        return handleUse(command);
    case CommandType::Inventory:
        return handleInventory();
    case CommandType::Talk:
        return handleTalk(command);
    case CommandType::Ask:
        return handleAsk(command);
    case CommandType::Save:
        return handleSave();
    case CommandType::Load:
        return handleLoad();
    case CommandType::Quit:
        running_ = false;
        return "게임을 종료합니다.\n";
    case CommandType::Help:
        return getHelpText();
    case CommandType::Unknown:
    default:
        return "알 수 없는 명령어입니다. 'help'를 입력하면 사용할 수 있는 명령어를 볼 수 있습니다.\n";
    }
}

bool GameEngine::isRunning() const
{
    return running_;
}

std::string GameEngine::handleLook() const
{
    return formatRoom(world_.getRoom(player_.getCurrentRoomId()));
}

std::string GameEngine::handleMove(const Command& command)
{
    if (command.arguments.empty())
    {
        return "이동할 방향을 입력해야 합니다. 예: go north\n";
    }

    const Room& currentRoom = world_.getRoom(player_.getCurrentRoomId());
    if (player_.getCurrentRoomId() == "castle_gate" &&
        command.arguments[0] == "south" &&
        !state_.isCastleGateUnlocked())
    {
        return "성문은 굳게 닫혀 있다. 열쇠가 필요하다.\n";
    }

    const std::optional<std::string> nextRoomId = currentRoom.getExitRoomId(command.arguments[0]);
    if (!nextRoomId.has_value())
    {
        return "그 방향으로는 이동할 수 없습니다.\n";
    }

    player_.setCurrentRoomId(nextRoomId.value());

    std::ostringstream output;
    if (player_.getCurrentRoomId() == "castle_courtyard")
    {
        output << "당신은 성 안뜰로 들어갔다.\n";
        if (!state_.hasReachedCourtyard())
        {
            state_.setReachedCourtyard(true);
            output << "첫 번째 목표를 달성했다!\n";
        }
    }

    output << formatRoom(world_.getRoom(player_.getCurrentRoomId()));
    return output.str();
}

std::string GameEngine::handleNpc() const
{
    const Room& room = world_.getRoom(player_.getCurrentRoomId());
    if (room.getNpcs().empty())
    {
        return "이 장소에는 대화할 수 있는 NPC가 없습니다.\n";
    }

    std::ostringstream output;
    output << "이 장소의 NPC:\n";
    for (const NPC& npc : room.getNpcs())
    {
        output << "- " << npc.getId() << ": " << npc.getName() << "\n";
    }

    return output.str();
}

std::string GameEngine::handleItems() const
{
    const Room& room = world_.getRoom(player_.getCurrentRoomId());
    if (room.getItems().empty())
    {
        return "이 장소에는 주울 수 있는 아이템이 없습니다.\n";
    }

    std::ostringstream output;
    output << "이 장소의 아이템:\n";
    for (const Item& item : room.getItems())
    {
        output << "- " << item.getId() << ": " << item.getName() << " - " << item.getDescription() << "\n";
    }

    return output.str();
}

std::string GameEngine::handleTake(const Command& command)
{
    if (command.arguments.empty())
    {
        return "주울 아이템 ID를 입력해야 합니다. 예: take old_key\n";
    }

    Room& room = world_.getRoom(player_.getCurrentRoomId());
    std::optional<Item> item = room.takeItem(command.arguments[0]);
    if (!item.has_value())
    {
        return "이 장소에는 그런 아이템이 없습니다.\n";
    }

    player_.addItem(item.value());

    std::ostringstream output;
    output << "아이템을 획득했습니다: " << item->getName() << "\n";
    return output.str();
}

std::string GameEngine::handleUse(const Command& command)
{
    if (command.arguments.empty())
    {
        return "사용할 아이템 ID를 입력해야 합니다. 예: use old_key\n";
    }

    const std::string& itemId = command.arguments[0];
    if (!player_.hasItem(itemId))
    {
        return "그런 아이템을 가지고 있지 않다.\n";
    }

    if (itemId != "old_key")
    {
        return "여기서는 그 아이템을 사용할 수 없다.\n";
    }

    if (player_.getCurrentRoomId() != "castle_gate")
    {
        return "여기서는 낡은 열쇠를 사용할 수 없다.\n";
    }

    if (state_.isCastleGateUnlocked())
    {
        return "성문은 이미 열려 있다.\n";
    }

    state_.unlockCastleGate();
    return "낡은 열쇠를 사용해 성문을 열었다.\n";
}

std::string GameEngine::handleInventory() const
{
    if (player_.getInventory().empty())
    {
        return "인벤토리가 비어 있습니다.\n";
    }

    std::ostringstream output;
    output << "인벤토리:\n";
    for (const Item& item : player_.getInventory())
    {
        output << "- " << item.getId() << ": " << item.getName() << "\n";
    }

    return output.str();
}

std::string GameEngine::handleTalk(const Command& command) const
{
    if (command.arguments.empty())
    {
        return "대화할 NPC ID를 입력해야 합니다. 예: talk merchant\n";
    }

    const std::string& npcId = command.arguments[0];
    const Room& room = world_.getRoom(player_.getCurrentRoomId());
    const NPC* npc = room.findNpc(npcId);

    if (npc == nullptr)
    {
        return "이 장소에는 그런 NPC가 없습니다.\n";
    }

    return npc->getFixedDialogue() + "\n";
}

std::string GameEngine::handleAsk(const Command& command) const
{
    if (command.target.empty())
    {
        return "누구에게 물어볼지 입력해라.\n";
    }

    std::istringstream targetStream(command.target);
    std::string npcId;
    targetStream >> npcId;

    std::string question;
    std::getline(targetStream, question);
    const std::size_t firstQuestionChar = question.find_first_not_of(" \t\r\n");
    if (firstQuestionChar != std::string::npos)
    {
        question = question.substr(firstQuestionChar);
    }
    else
    {
        question.clear();
    }

    if (npcId.empty())
    {
        return "누구에게 물어볼지 입력해라.\n";
    }

    if (question.empty())
    {
        return "무엇을 물어볼지 입력해라.\n";
    }

    const Room& room = world_.getRoom(player_.getCurrentRoomId());
    const NPC* npc = room.findNpc(npcId);

    if (npc == nullptr)
    {
        return "이곳에는 그런 NPC가 없다.\n";
    }

    if (!npc->isAiEnabled())
    {
        return "이 NPC는 질문에 답할 수 없다.\n";
    }

    const std::string prompt = buildNPCPrompt(*npc, question);
    const std::string response = llmClient_.generateResponse(prompt);

    std::ostringstream output;
    output << npc->getName() << ": ";
    if (response.empty())
    {
        output << "지금은 대답하기 어렵군.";
    }
    else
    {
        output << response;
    }
    output << "\n";
    return output.str();
}

std::string GameEngine::handleSave() const
{
    if (saveManager_.save(player_, state_, saveFilePath_))
    {
        return "게임을 저장했다.\n";
    }

    return "게임 저장에 실패했다.\n";
}

std::string GameEngine::handleLoad()
{
    Player loadedPlayer;
    GameState loadedState;

    if (!saveManager_.load(loadedPlayer, loadedState, saveFilePath_))
    {
        return "게임 불러오기에 실패했다.\n";
    }

    if (!world_.hasRoom(loadedPlayer.getCurrentRoomId()))
    {
        return "게임 불러오기에 실패했다.\n";
    }

    world_ = World::createDefaultWorld();
    player_ = loadedPlayer;
    state_ = loadedState;
    world_.removeInventoryItemsFromRooms(player_.getInventory());

    std::ostringstream output;
    output << "게임을 불러왔다.\n";
    output << formatRoom(world_.getRoom(player_.getCurrentRoomId()));
    return output.str();
}

std::string GameEngine::getHelpText() const
{
    std::ostringstream output;
    output << "\n사용 가능한 명령어\n";
    output << "- look: 현재 장소를 살펴봅니다.\n";
    output << "- go north 또는 north: 북쪽으로 이동합니다.\n";
    output << "- go south 또는 south: 남쪽으로 이동합니다.\n";
    output << "- go east 또는 east: 동쪽으로 이동합니다.\n";
    output << "- go west 또는 west: 서쪽으로 이동합니다.\n";
    output << "- npc 또는 npcs: 현재 장소의 NPC를 확인합니다.\n";
    output << "- talk NPC_ID: NPC와 대화합니다. 예: talk merchant 또는 talk guard\n";
    output << "- ask NPC_ID 질문: NPC에게 질문합니다. 예: ask guard 성 안에는 뭐가 있나요?\n";
    output << "- items: 현재 장소의 아이템을 확인합니다.\n";
    output << "- take ITEM_ID: 아이템을 줍습니다. 예: take old_key\n";
    output << "- use ITEM_ID: 아이템을 사용합니다. 예: use old_key\n";
    output << "- inventory: 인벤토리를 확인합니다.\n";
    output << "- save: 현재 게임 상태를 저장합니다.\n";
    output << "- load: 저장된 게임 상태를 불러옵니다.\n";
    output << "- quit: 게임을 종료합니다.\n";
    return output.str();
}

std::string GameEngine::buildNPCPrompt(const NPC& npc, const std::string& question) const
{
    const Room& room = world_.getRoom(player_.getCurrentRoomId());

    std::ostringstream prompt;
    prompt << "너는 텍스트 MUD 게임의 NPC다.\n\n";
    prompt << "NPC 이름: " << npc.getName() << "\n";
    prompt << "NPC 성격: " << npc.getPersonality() << "\n";
    prompt << "NPC가 알고 있는 정보:\n" << npc.getKnowledge() << "\n\n";

    prompt << "현재 게임 상태:\n";
    prompt << "- 현재 방 ID: " << room.getId() << "\n";
    prompt << "- 현재 방: " << room.getName() << "\n";
    prompt << "- 현재 방 설명: " << room.getDescription() << "\n";
    prompt << "- 플레이어 인벤토리: " << formatInventoryForPrompt() << "\n";
    prompt << "- 성문 잠금 해제 여부: " << (state_.isCastleGateUnlocked() ? "true" : "false") << "\n";
    prompt << "- 성 안뜰 도착 여부: " << (state_.hasReachedCourtyard() ? "true" : "false") << "\n\n";

    prompt << "월드 정보:\n";
    prompt << "- 마을 광장 동쪽에는 잡화점이 있다.\n";
    prompt << "- 마을 광장 남쪽에는 성문이 있다.\n";
    prompt << "- 잡화점에는 낡은 열쇠가 있었다.\n";
    prompt << "- 성문은 낡은 열쇠를 사용해야 열 수 있다.\n";
    prompt << "- 성문 너머에는 성 안뜰이 있다.\n\n";

    prompt << "규칙:\n";
    prompt << "1. NPC의 입장에서만 대답한다.\n";
    prompt << "2. 게임에 없는 장소, 아이템, NPC를 새로 만들지 않는다.\n";
    prompt << "3. 플레이어에게 직접 명령어를 실행해주지 않는다.\n";
    prompt << "4. 게임 상태를 변경했다고 말하지 않는다.\n";
    prompt << "5. 아이템을 주거나 문을 열어주지 않는다.\n";
    prompt << "6. 답변은 3문장 이내로 한다.\n";
    prompt << "7. 힌트는 줄 수 있지만 정답을 너무 직접적으로 말하지 않는다.\n\n";

    prompt << "플레이어 질문:\n";
    prompt << question << "\n\n";
    prompt << "NPC 대답:\n";
    return prompt.str();
}

std::string GameEngine::formatInventoryForPrompt() const
{
    if (player_.getInventory().empty())
    {
        return "없음";
    }

    std::ostringstream inventory;
    for (std::size_t i = 0; i < player_.getInventory().size(); ++i)
    {
        if (i > 0)
        {
            inventory << ", ";
        }
        inventory << player_.getInventory()[i].getId();
    }

    return inventory.str();
}

std::string GameEngine::formatRoom(const Room& room) const
{
    std::ostringstream output;
    output << "\n[" << room.getName() << "]\n";
    output << room.getDescription() << "\n";

    output << "이동 가능:";
    for (const auto& exit : room.getExits())
    {
        output << " " << exit.first;
    }
    output << "\n";

    return output.str();
}
