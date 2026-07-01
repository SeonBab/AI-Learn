# 텍스트 MUD 첫 플레이 루프 구현 작업 기록

## 설명

사용자는 이전에 말했던 첫 번째 작업을 진행해달라고 요청했다.

이번 작업에서는 `docs/text-mud-first-play-loop.md`에 정리한 첫 플레이 루프와 첫 번째 시나리오 `숲길의 작은 약초`를 `TextMUDGame` 프로젝트의 최소 실행 코드로 구현했다.

## 핵심 정리

## 사용자가 지시한 내용

1. MUD 게임의 첫 번째 작업을 진행한다.
2. 첫 플레이 루프와 첫 번째 플레이 가능한 시나리오를 실제 프로젝트에 반영한다.

## assistant가 작업한 내용

1. `TextMUDGame/src/main.cpp`에 최소 월드 데이터를 추가했다.
2. `Room`, `Item`, `Npc`, `Player` 구조체를 추가했다.
3. `마을 입구`, `숲길`, `상점` 3개 방을 구현했다.
4. `herb` 아이템과 `villager`, `merchant` NPC를 구현했다.
5. `look`, `north`, `south`, `east`, `west`, `npc`, `talk`, `items`, `take`, `inventory`, `give`, `quit`, `help` 명령어를 처리하도록 했다.
6. `give herb villager` 명령어로 첫 번째 목표를 완료하는 흐름을 구현했다.
7. 플레이어에게 보이는 출력 문구는 한글로 유지했다.
8. `cl.exe` 직접 호출로 컴파일을 확인했다.
9. 기본 진행 흐름을 자동 입력으로 실행해 첫 목표 완료까지 확인했다.
10. 직접 컴파일 과정에서 생기는 임시 산출물이 Git 작업 상태에 보이지 않도록 `.gitignore`를 보강했다.

## assistant가 답변한 핵심 내용

1. 첫 playable 흐름은 `마을 입구 -> 숲길 -> 약초 획득 -> 마을 입구 -> 마을 사람에게 전달`이다.
2. 로컬 LLM NPC는 아직 붙이지 않고, 고정 대사 기반으로 전체 흐름을 먼저 완성했다.
3. 다음 단계에서 이 코드를 `GameEngine`, `CommandParser`, `World` 파일로 분리할 수 있다.

## 생성하거나 수정한 파일

| 파일 | 작업 내용 |
|---|---|
| `TextMUDGame/src/main.cpp` | 첫 플레이 루프와 첫 번째 시나리오 구현 |
| `.gitignore` | 직접 컴파일 산출물 ignore 규칙 추가 |
| `docs/work-logs/text-mud-first-play-loop-implementation.md` | 이번 작업 기록 생성 |

## 검증 결과

`MSBuild`는 현재 Codex 셸 환경의 `PATH`/`Path` 중복 문제로 실패했다.

```text
error MSB6001: "CL.exe"의 명령줄 스위치가 잘못되었습니다.
System.ArgumentException: 항목이 이미 추가되었습니다. 사전에 있는 키: 'PATH' 추가되는 키: 'Path'
```

대신 Visual Studio Developer Command 환경에서 `cl.exe`를 직접 호출해 컴파일을 확인했다.

```text
cl /nologo /EHsc /std:c++17 /utf-8 /I TextMUDGame\src TextMUDGame\src\main.cpp /Fe:TextMUDGame\x64\Debug\TextMUDGame.exe
```

다음 자동 입력 흐름으로 첫 목표 완료까지 확인했다.

```text
look
npc
talk villager
north
look
items
take herb
south
give herb villager
quit
```

확인 결과 `첫 번째 목표를 완료했습니다.` 메시지가 정상 출력되었다.

## 다음에 이어서 할 수 있는 작업

1. `main.cpp`의 구조체와 명령 처리 코드를 `GameEngine`, `CommandParser`, `World`로 분리한다.
2. `villager` NPC에 로컬 LLM 응답 기능을 연결한다.
3. `go north`, `npcs`, 한글 명령어 별칭을 추가한다.
4. Visual Studio에서 `TextMUDGame.sln`을 빌드하고 한글 출력과 플레이 흐름을 확인한다.
