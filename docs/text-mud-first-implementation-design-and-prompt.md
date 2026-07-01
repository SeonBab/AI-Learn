# C++ MUD 게임 1차 구현 설계 문서와 코드 요청 프롬프트

## 설명

이 문서는 C++ 학습용 로컬 AI 채팅 프로그램 확장 목표와 텍스트 기반 MUD 게임 제작 목표를 기준으로, 1차 MUD 구현 설계와 코드 요청 프롬프트를 함께 정리한다.

1차 구현의 목표는 AI 기능 없이도 동작하는 기본 MUD 게임 엔진을 만드는 것이다. 로컬 LLM 연동, 저장 기능, 전투 같은 확장 기능은 뒤로 미루고, 먼저 `명령어 입력 -> 게임 상태 변경 -> 결과 출력` 구조를 안정적으로 만든다.

## 핵심 정리

1. 1차 게임 제목은 `낡은 성문의 비밀`이다.
2. 플레이어는 `마을 광장`에서 시작한다.
3. 기본 기능은 방 설명 보기, 이동, 아이템 확인, 아이템 줍기, 인벤토리 확인, NPC 고정 대사 대화, 도움말, 종료다.
4. 로컬 LLM 기반 AI NPC, `save/load`, 전투, 레벨, 장비, 멀티플레이는 1차 구현에서 제외한다.
5. C++ 클래스는 `GameEngine`, `World`, `Room`, `Item`, `NPC`, `Player`, `CommandParser`, `Command`를 기준으로 한다.

## 1차 구현 기능

1차 버전에서 구현할 기능은 다음과 같다.

```text
1. 게임 시작
2. 현재 방 설명 보기
3. 방 이동하기
4. 방에 있는 아이템 확인
5. 아이템 줍기
6. 인벤토리 확인
7. 현재 방의 NPC와 대화하기
8. 도움말 보기
9. 게임 종료하기
```

사용할 명령어는 다음과 같다.

```text
look
go north
go south
go east
go west
items
take old_key
inventory
talk guard
talk merchant
help
quit
```

## 이번 단계에서 제외할 기능

```text
1. 로컬 LLM 기반 AI NPC 응답
2. save / load 저장 기능
3. 전투
4. 체력
5. 레벨
6. 경험치
7. 장비
8. 상점 구매
9. 복잡한 퀘스트
10. 멀티플레이
11. 네트워크 통신
```

## 1차 게임 시나리오

게임 제목은 다음과 같다.

```text
낡은 성문의 비밀
```

플레이어는 `마을 광장`에서 시작한다. 동쪽에는 `잡화점`이 있고, 남쪽에는 `성문 앞`이 있다. 잡화점에는 `낡은 열쇠`가 있다. 성문 앞에는 `경비병`이 있으며, 플레이어에게 성문에 대한 힌트를 준다.

1차 버전에서는 실제로 열쇠로 문을 여는 기능까지 구현하지 않는다. 우선 이동, 아이템 획득, NPC 대화가 정상 작동하는 것을 목표로 한다.

## 초기 방 구성

| 방 ID | 이름 | 설명 | 이동 가능 | 아이템 | NPC |
|---|---|---|---|---|---|
| `village_square` | 마을 광장 | 작은 마을의 중심지다. 동쪽에는 잡화점이 있고, 남쪽에는 성문으로 가는 길이 있다. | `east -> general_store`, `south -> castle_gate` | 없음 | 없음 |
| `general_store` | 잡화점 | 오래된 나무 선반 위에 여러 물건이 놓여 있다. | `west -> village_square` | `old_key` | `merchant` |
| `castle_gate` | 성문 앞 | 거대한 성문이 굳게 닫혀 있다. 누군가 이곳을 지키고 있다. | `north -> village_square` | 없음 | `guard` |

## 초기 아이템 구성

| 아이템 ID | 이름 | 설명 |
|---|---|---|
| `old_key` | 낡은 열쇠 | 성문을 열 수 있을 것 같은 오래된 열쇠다. |

1차 버전에서는 아이템 사용 기능을 제외한다. 따라서 `old_key`는 줍고 인벤토리에서 확인하는 기능까지만 구현한다.

## 초기 NPC 구성

| NPC ID | 이름 | 대사 |
|---|---|---|
| `merchant` | 상인 | 필요한 물건이 있다면 주변을 잘 살펴보게. |
| `guard` | 경비병 | 성문은 아무나 통과할 수 없다. 오래된 열쇠가 있다면 이야기가 달라질지도 모르지. |

## 필요한 클래스

1차 구현에 필요한 클래스는 다음과 같다.

```text
GameEngine
World
Room
Item
NPC
Player
CommandParser
Command
```

나중에 추가할 클래스는 다음과 같다.

```text
LLMClient
SaveManager
GameState
QuestManager
```

## 명령어 처리 흐름

전체 흐름은 다음과 같다.

```text
1. main.cpp에서 GameEngine 객체 생성
2. GameEngine 실행
3. 게임 시작 메시지 출력
4. 현재 방 설명 출력
5. 사용자 입력 받기
6. CommandParser가 입력을 Command로 변환
7. GameEngine이 action 값에 따라 처리 함수 호출
8. World와 Player 상태를 확인하거나 수정
9. 결과 문자열 반환
10. 콘솔에 결과 출력
11. quit 입력 전까지 반복
```

## 1차 구현 완료 기준

아래 플레이 흐름이 정상 작동하면 1차 구현은 성공이다.

```text
look
go east
look
items
take old_key
inventory
talk merchant
go west
go south
talk guard
help
quit
```

예상 결과는 다음과 같다.

```text
- 플레이어가 마을 광장에서 시작한다.
- 동쪽으로 이동하면 잡화점에 도착한다.
- 잡화점에서 낡은 열쇠를 볼 수 있다.
- take old_key로 낡은 열쇠를 획득할 수 있다.
- inventory에서 낡은 열쇠를 확인할 수 있다.
- 상인과 대화할 수 있다.
- 다시 마을 광장으로 돌아와 성문 앞으로 이동할 수 있다.
- 경비병과 대화할 수 있다.
- help로 명령어 목록을 볼 수 있다.
- quit으로 게임을 종료할 수 있다.
```

## 코드 요청 프롬프트

다음 프롬프트는 1차 MUD 코드 구현 요청에 사용할 수 있다.

```text
너는 C++ 학습을 도와주는 프로그래밍 assistant다.

아래 설계 문서를 바탕으로 콘솔에서 실행되는 텍스트 기반 MUD 게임의 1차 버전을 구현해줘.

구현 목표:
AI 기능 없이도 동작하는 기본 MUD 게임 엔진을 만들어줘.

사용할 명령어:
look
go north
go south
go east
go west
items
take old_key
inventory
talk guard
talk merchant
help
quit

게임 제목:
낡은 성문의 비밀

방 구성:
- village_square: 마을 광장, east -> general_store, south -> castle_gate
- general_store: 잡화점, west -> village_square, item old_key, NPC merchant
- castle_gate: 성문 앞, north -> village_square, NPC guard

아이템:
- old_key: 낡은 열쇠

NPC:
- merchant: 필요한 물건이 있다면 주변을 잘 살펴보게.
- guard: 성문은 아무나 통과할 수 없다. 오래된 열쇠가 있다면 이야기가 달라질지도 모르지.

필요한 클래스:
GameEngine, World, Room, Item, NPC, Player, CommandParser, Command

코드 작성 규칙:
1. C++17 기준으로 작성한다.
2. using namespace std;는 사용하지 않는다.
3. 가능한 한 함수가 바로 cout 하지 말고 결과 문자열을 반환한다.
4. GameEngine::processCommand()는 문자열을 반환한다.
5. 잘못된 명령어, 존재하지 않는 방향, 아이템, NPC를 입력해도 프로그램이 종료되지 않게 한다.
6. std::map, std::vector, std::string 중심으로 구현한다.

완료 기준:
look
go east
look
items
take old_key
inventory
talk merchant
go west
go south
talk guard
help
quit
```

## 다음 확장 단계

1차 구현이 끝나면 다음 순서로 확장한다.

```text
2차: use old_key 명령어와 잠긴 성문 열기
3차: save / load 기능 추가
4차: ask guard 질문 기능 추가
5차: LLMClient를 통해 Ollama 로컬 LLM 연동
6차: 퀘스트 상태 추가
7차: 방, 아이템, NPC 데이터 파일 분리
8차: 네트워크 확장 구조 검토
```
