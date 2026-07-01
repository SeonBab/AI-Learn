# C++ MUD 게임 프로젝트 구조 설계

## 설명

이 문서는 텍스트 MUD 게임을 어떤 C++ 클래스와 파일로 나눌지 설계한다.

현재 게임의 기본 흐름은 `명령어 입력 -> 게임 상태 변경 -> 결과 출력 -> AI NPC 대화`이다. 첫 구현은 `main.cpp`에 최소 플레이 루프가 들어가 있지만, 다음 단계부터는 학습하기 좋은 C++ 객체 구조로 분리해야 한다.

이번 설계의 목적은 바로 코드를 늘리는 것이 아니라, 다음 질문에 답하는 것이다.

```text
이 게임은 어떤 객체들로 구성되는가?
각 객체는 어떤 데이터를 가지는가?
각 객체는 어떤 기능을 담당하는가?
명령어가 입력되면 어떤 순서로 처리되는가?
```

## 핵심 정리

1. `main.cpp`는 콘솔 입출력과 프로그램 시작점만 담당한다.
2. 실제 게임 진행은 `GameEngine`이 관리한다.
3. 입력 문자열 분석은 `CommandParser`가 담당한다.
4. 방, 아이템, NPC 같은 고정 데이터는 `World`가 가진다.
5. 현재 위치와 인벤토리 같은 진행 상태는 `Player`가 가진다.
6. 로컬 LLM 호출은 나중에 `LLMClient`로 분리한다.
7. 첫 번째 리팩터링 범위는 `look`, `go`, `take`, `inventory`, `talk`, `quit`까지만 잡는다.

## 전체 객체 구성

초기 구조는 다음 객체들로 구성한다.

| 클래스 | 핵심 역할 |
|---|---|
| `GameEngine` | 게임 전체 진행과 명령어 처리 결과 생성 |
| `Player` | 플레이어 현재 위치와 인벤토리 관리 |
| `Room` | 장소 정보, 이동 경로, 방 안의 아이템과 NPC 관리 |
| `Item` | 아이템 ID, 이름, 설명 관리 |
| `NPC` | NPC ID, 이름, 고정 대사, AI 사용 여부 관리 |
| `World` | 전체 방 데이터와 검색 기능 관리 |
| `CommandParser` | 입력 문자열을 명령어 객체로 변환 |
| `LLMClient` | Ollama 같은 로컬 LLM 호출 담당 |

초기에는 `LLMClient`를 설계만 해두고 실제 연결은 뒤로 미룬다. 먼저 고정 대사 기반 플레이 루프가 안정적으로 동작해야 한다.

## 권장 파일 구조

`TextMUDGame/src` 아래 파일 구조는 다음처럼 나눈다.

```text
TextMUDGame/
  TextMUDGame.sln
  TextMUDGame.vcxproj
  src/
    main.cpp
    GameEngine.h
    GameEngine.cpp
    Player.h
    Player.cpp
    Room.h
    Room.cpp
    Item.h
    Item.cpp
    NPC.h
    NPC.cpp
    World.h
    World.cpp
    CommandParser.h
    CommandParser.cpp
    LLMClient.h
    LLMClient.cpp
```

파일이 늘어나더라도 각 파일의 책임은 작게 유지한다. 예를 들어 `Room`은 방 데이터를 표현하고, 명령어 처리 로직은 `GameEngine`에 둔다.

## 클래스별 책임과 데이터

### GameEngine

`GameEngine`은 게임 진행의 중심이다.

담당 책임은 다음과 같다.

1. `World`, `Player`, `CommandParser`를 가지고 있다.
2. 사용자 명령어를 받아 처리한다.
3. 명령어 결과를 문자열로 반환한다.
4. 게임 종료 여부를 관리한다.
5. 나중에 `LLMClient`를 통해 AI NPC 응답을 요청한다.

주요 데이터는 다음과 같다.

```text
World world
Player player
CommandParser parser
bool running
```

주요 기능은 다음과 같다.

```text
start()
processCommand(inputText) -> string
isRunning() -> bool
getIntroText() -> string
```

초기에는 `processCommand`가 다음 명령어만 처리한다.

```text
look
go north
go south
go east
go west
north
south
east
west
items
take old_key
inventory
talk merchant
quit
```

### Player

`Player`는 현재 플레이어 상태를 관리한다.

담당 책임은 다음과 같다.

1. 현재 방 ID를 가진다.
2. 인벤토리를 가진다.
3. 아이템 보유 여부를 확인한다.
4. 아이템 추가와 제거를 처리한다.
5. 나중에 퀘스트 진행 상태를 가진다.

주요 데이터는 다음과 같다.

```text
string currentRoomId
vector<Item> inventory
bool firstQuestCompleted
```

주요 기능은 다음과 같다.

```text
getCurrentRoomId() -> string
setCurrentRoomId(roomId)
addItem(item)
removeItem(itemId) -> bool
hasItem(itemId) -> bool
getInventoryText() -> string
```

### Room

`Room`은 게임 안의 장소 하나를 표현한다.

담당 책임은 다음과 같다.

1. 방 ID, 이름, 설명을 가진다.
2. 이동 가능한 방향과 연결된 방 ID를 가진다.
3. 방 안의 아이템 목록을 가진다.
4. 방 안의 NPC 목록을 가진다.
5. 아이템 획득 시 방에서 아이템을 제거한다.

주요 데이터는 다음과 같다.

```text
string id
string name
string description
map<string, string> exits
vector<Item> items
vector<NPC> npcs
```

주요 기능은 다음과 같다.

```text
getDescriptionText() -> string
getExitRoomId(direction) -> optional string
getItemsText() -> string
takeItem(itemId) -> optional Item
getNpcsText() -> string
findNpc(npcId) -> optional NPC
```

### Item

`Item`은 플레이어가 얻거나 사용할 수 있는 물건이다.

담당 책임은 다음과 같다.

1. 아이템 ID를 가진다.
2. 출력용 이름을 가진다.
3. 설명을 가진다.
4. 나중에 사용 가능 여부와 효과를 가진다.

주요 데이터는 다음과 같다.

```text
string id
string name
string description
```

초기 기능은 단순 조회만 있으면 충분하다.

```text
getId() -> string
getName() -> string
getDescription() -> string
```

### NPC

`NPC`는 대화 가능한 캐릭터를 표현한다.

담당 책임은 다음과 같다.

1. NPC ID와 이름을 가진다.
2. 고정 대사를 가진다.
3. AI 응답 사용 여부를 가진다.
4. 나중에 AI 프롬프트용 성격과 지식 정보를 가진다.

주요 데이터는 다음과 같다.

```text
string id
string name
string fixedDialogue
bool usesAI
string personality
vector<string> knownFacts
```

초기 기능은 다음과 같다.

```text
getDialogue(player, world) -> string
usesAIResponse() -> bool
```

첫 단계에서는 `usesAI`가 `false`인 고정 대사 NPC만 실제 구현한다.

### World

`World`는 전체 게임 월드 데이터를 관리한다.

담당 책임은 다음과 같다.

1. 모든 방을 가진다.
2. 방 ID로 방을 찾는다.
3. 초기 월드를 생성한다.
4. 이동 가능한 방인지 확인할 수 있게 한다.
5. 나중에 저장 데이터와 월드 데이터를 분리한다.

주요 데이터는 다음과 같다.

```text
map<string, Room> rooms
```

주요 기능은 다음과 같다.

```text
createDefaultWorld() -> World
getRoom(roomId) -> Room&
hasRoom(roomId) -> bool
```

첫 번째 월드는 다음 방을 가진다.

```text
village_square
general_store
castle_gate
```

### CommandParser

`CommandParser`는 사용자 입력 문자열을 분석한다.

담당 책임은 다음과 같다.

1. 앞뒤 공백을 제거한다.
2. 입력 문자열을 단어 단위로 나눈다.
3. 명령어 종류와 인자를 구분한다.
4. 잘못된 명령어를 `Unknown`으로 분류한다.

명령어 분석 결과는 별도 구조체로 둔다.

```text
Command
- type
- arguments
- originalText
```

명령어 종류는 다음 enum으로 표현할 수 있다.

```cpp
enum class CommandType
{
    Look,
    Move,
    Items,
    Take,
    Inventory,
    Talk,
    Ask,
    Save,
    Quit,
    Help,
    Unknown
};
```

초기 기능은 다음과 같다.

```text
parse(inputText) -> Command
```

첫 구현에서는 `go north`와 `north`를 모두 `Move`로 처리하는 것이 좋다. 기존 첫 플레이 루프 문서의 짧은 명령어와 전체 기획 문서의 `go 방향` 형식을 함께 받아들일 수 있기 때문이다.

### LLMClient

`LLMClient`는 로컬 LLM 호출을 담당한다.

담당 책임은 다음과 같다.

1. Ollama API 호출을 감싼다.
2. NPC 대화 프롬프트를 전달한다.
3. 응답 문자열을 반환한다.
4. LLM 호출 실패 시 오류 메시지를 반환한다.

주요 데이터는 다음과 같다.

```text
string endpoint
string modelName
```

주요 기능은 다음과 같다.

```text
generate(prompt) -> string
```

첫 번째 구조 분리 단계에서는 실제 구현하지 않고 인터페이스 형태만 만든다. 기존 `LocalAIChat`의 Ollama 호출 구조를 참고해 나중에 옮긴다.

## 명령어 처리 흐름

사용자가 명령어를 입력하면 다음 순서로 처리한다.

```text
main.cpp
-> GameEngine::processCommand(inputText)
-> CommandParser::parse(inputText)
-> CommandType에 따라 GameEngine 내부 처리 함수 호출
-> World와 Player 상태 조회 또는 변경
-> 결과 메시지 string 생성
-> main.cpp가 cout으로 출력
```

예를 들어 `take old_key` 명령어는 다음 순서로 처리된다.

```text
1. main.cpp가 "take old_key" 입력을 받는다.
2. GameEngine::processCommand("take old_key")를 호출한다.
3. CommandParser가 CommandType::Take와 argument "old_key"를 만든다.
4. GameEngine이 Player의 현재 방 ID를 확인한다.
5. World에서 현재 Room을 가져온다.
6. Room에서 old_key 아이템을 찾고 제거한다.
7. Player 인벤토리에 old_key를 추가한다.
8. "아이템을 획득했습니다: 낡은 열쇠" 문자열을 반환한다.
9. main.cpp가 결과를 출력한다.
```

## 첫 번째 구현 범위

구조 분리 후 첫 구현 범위는 다음으로 제한한다.

| 기능 | 포함 여부 | 이유 |
|---|---|---|
| `look` | 포함 | 현재 방 설명 출력의 기본 |
| `go` 또는 방향 이동 | 포함 | 방 이동 학습 |
| `items` | 포함 | 방 데이터 조회 |
| `take` | 포함 | 방 상태와 인벤토리 변경 |
| `inventory` | 포함 | 플레이어 상태 조회 |
| `talk` | 포함 | NPC 고정 대사 출력 |
| `quit` | 포함 | 실행 루프 종료 |
| `ask` | 설계만 | AI NPC는 구조 안정화 후 연결 |
| `save` | 설계만 | 파일 입출력은 다음 단계로 분리 |

첫 번째 구조 분리에서는 `ask`, `save`, `load`를 실제 구현하지 않는다. 대신 `CommandType`에는 미리 넣어두어 나중에 확장하기 쉽게 한다.

## main.cpp의 역할

`main.cpp`는 최대한 얇게 유지한다.

담당할 일은 다음뿐이다.

1. Windows 콘솔 UTF-8 설정
2. `GameEngine` 생성
3. 시작 안내 출력
4. 사용자 입력 반복
5. `GameEngine` 결과 출력

예상 구조는 다음과 같다.

```cpp
// C++ 예제 코드는 이 형식으로 작성한다.
int main()
{
    setupConsoleEncoding();

    GameEngine game;
    std::cout << game.getIntroText();

    std::string input;
    while (game.isRunning() && std::getline(std::cin, input))
    {
        std::cout << game.processCommand(input);
    }

    return 0;
}
```

## 설계상 결정한 내용

이번 설계에서는 다음 방향을 채택한다.

1. `main.cpp`에는 게임 규칙을 두지 않는다.
2. `GameEngine::processCommand`는 결과를 `string`으로 반환한다.
3. `Room`, `Item`, `NPC`는 우선 값 객체처럼 단순하게 둔다.
4. `World`는 초기 데이터를 코드로 생성한다.
5. 저장 파일과 외부 데이터 로딩은 나중에 추가한다.
6. AI NPC 호출은 `LLMClient`로 분리하되 첫 구조 분리에서는 실제 호출하지 않는다.
7. 영어 명령어는 유지하되 출력 문장은 한글로 작성한다.

## 애매하거나 다음에 결정할 부분

다음 항목은 구현 전에 한 번 더 결정하면 좋다.

1. `go north`와 `north`를 둘 다 지원할지 여부
2. `npc`와 `npcs`를 둘 다 지원할지 여부
3. `use old_key`를 2차 구현 범위에 포함할지 여부
4. `ask guard 질문`을 언제 실제 LLM과 연결할지 여부
5. 월드 데이터를 코드에 둘지, 나중에 JSON 또는 텍스트 파일로 분리할지 여부

현재 추천은 다음과 같다.

```text
첫 구조 분리: look, go/north, items, take, inventory, talk, quit
다음 확장: give, ask, save, load
```

## 다음 작업

다음 작업은 이 설계 문서를 기준으로 첫 번째 코드 구현 요청 프롬프트를 만드는 것이다.

구현 요청은 다음 순서로 나누는 것이 좋다.

1. `Item`, `NPC`, `Room`, `Player`, `World` 파일 분리
2. `CommandParser` 추가
3. `GameEngine` 추가
4. `main.cpp`를 얇게 정리
5. 기존 첫 플레이 루프가 그대로 동작하는지 빌드와 실행으로 확인
