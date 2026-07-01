# C++ MUD 게임 2차 구현 작업 기록

## 설명

사용자는 첨부한 2차 작업 설계 문서와 코드 요청 프롬프트를 읽고, 설계 문서를 정리한 뒤 실제 작업을 진행해달라고 요청했다.

이번 작업에서는 2차 구현 기준 문서를 추가하고, `TextMUDGame`에 `use old_key`, 성문 잠금 해제, `GameState`, 성 안뜰 이동 기능을 구현했다.

## 핵심 정리

## 사용자가 지시한 내용

1. 첨부된 2차 작업 설계 문서와 코드 요청 프롬프트를 읽는다.
2. 설계 문서를 프로젝트 문서로 정리한다.
3. 2차 작업을 코드에 적용한다.

## assistant가 작업한 내용

1. `docs/text-mud-second-implementation-design-and-prompt.md` 문서를 생성했다.
2. `GameState.h` / `GameState.cpp`를 추가했다.
3. `CommandParser`에 `use` 명령어를 추가했다.
4. `GameEngine`에 `GameState` 멤버와 `handleUse()`를 추가했다.
5. `castle_gate`에서 `south` 이동 시 성문 잠금 상태를 검사하도록 했다.
6. `World`에 `castle_courtyard` 방을 추가했다.
7. `castle_gate`에 `south -> castle_courtyard` 이동 경로를 추가했다.
8. `TextMUDGame.vcxproj`와 `.filters`에 `GameState` 파일을 등록했다.
9. 정상 흐름과 실패 흐름을 컴파일 후 실행해 검증했다.

## 생성하거나 수정한 파일

| 파일 | 작업 내용 |
|---|---|
| `docs/text-mud-second-implementation-design-and-prompt.md` | 2차 구현 설계 문서와 코드 요청 프롬프트 생성 |
| `TextMUDGame/src/GameState.h` / `GameState.cpp` | 진행 상태 클래스 추가 |
| `TextMUDGame/src/CommandParser.h` / `CommandParser.cpp` | `use` 명령어 추가 |
| `TextMUDGame/src/GameEngine.h` / `GameEngine.cpp` | `handleUse`, 성문 잠금 검사, 목표 달성 메시지 추가 |
| `TextMUDGame/src/World.cpp` | `castle_courtyard` 방과 성문 남쪽 이동 경로 추가 |
| `TextMUDGame/TextMUDGame.vcxproj` | 새 소스/헤더 등록 |
| `TextMUDGame/TextMUDGame.vcxproj.filters` | Visual Studio 필터 등록 |

## 검증 결과

Visual Studio Developer Command 환경에서 `cl.exe`로 컴파일을 확인했다.

정상 흐름:

```text
look
go east
take old_key
go west
go south
use old_key
go south
look
quit
```

확인 결과 `낡은 열쇠를 사용해 성문을 열었다.`, `당신은 성 안뜰로 들어갔다.`, `첫 번째 목표를 달성했다!`가 정상 출력되었다.

실패 흐름도 확인했다.

| 테스트 | 확인 메시지 |
|---|---|
| 열쇠 없이 `use old_key` | `그런 아이템을 가지고 있지 않다.` |
| 잡화점에서 `use old_key` | `여기서는 낡은 열쇠를 사용할 수 없다.` |
| 잠긴 성문에서 `go south` | `성문은 굳게 닫혀 있다. 열쇠가 필요하다.` |
| `use wrong_item` | `그런 아이템을 가지고 있지 않다.` |
| 열린 성문에서 다시 `use old_key` | `성문은 이미 열려 있다.` |
| 성 안뜰 재방문 | 목표 달성 메시지는 다시 출력되지 않음 |

## 다음에 이어서 할 수 있는 작업

1. 3차 작업으로 `save / load` 저장 기능을 설계한다.
2. 저장 대상은 `currentRoomId`, `inventory`, `castleGateUnlocked`, `reachedCourtyard`를 우선 검토한다.
3. 저장 파일 형식을 단순 텍스트로 시작할지 JSON으로 시작할지 결정한다.
