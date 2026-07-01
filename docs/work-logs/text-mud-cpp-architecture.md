# C++ MUD 게임 프로젝트 구조 설계 작업 기록

## 설명

사용자는 MUD 게임의 다음 단계로 바로 코드를 작성하지 않고, 어떤 C++ 클래스와 파일로 나눌지 설계 문서를 만들라고 요청했다.

이번 작업에서는 `GameEngine`, `Player`, `Room`, `Item`, `NPC`, `World`, `CommandParser`, `LLMClient`의 책임과 데이터, 파일 구조, 명령어 처리 흐름, 첫 번째 구현 범위를 문서로 정리했다.

## 핵심 정리

## 사용자가 지시한 내용

1. 다음 작업 제목은 `C++ MUD 게임 프로젝트 구조 설계`로 잡는다.
2. MUD 게임을 어떤 C++ 클래스와 파일로 나눌지 설계한다.
3. 각 클래스의 책임을 정리한다.
4. 각 객체가 가지는 데이터와 담당 기능을 정리한다.
5. 명령어가 입력되면 어떤 순서로 처리되는지 정리한다.
6. 이번에는 코드 구현보다 설계 문서를 완성한다.

## assistant가 작업한 내용

1. 기존 MUD 게임 기획 문서와 첫 플레이 루프 문서를 확인했다.
2. `docs/text-mud-cpp-architecture.md` 설계 문서를 생성했다.
3. 핵심 클래스 8개를 정의했다.
4. 클래스별 책임, 주요 데이터, 주요 기능을 정리했다.
5. `TextMUDGame/src` 기준 권장 파일 구조를 정리했다.
6. `main.cpp -> GameEngine -> CommandParser -> World/Player -> 결과 문자열 반환` 흐름을 정리했다.
7. 첫 구조 분리에서 구현할 범위와 뒤로 미룰 범위를 나눴다.
8. README 문서 목록에 새 설계 문서를 추가했다.

## assistant가 답변한 핵심 내용

1. `main.cpp`는 콘솔 입출력과 프로그램 시작점만 담당해야 한다.
2. `GameEngine`은 게임 진행과 명령어 처리의 중심이 된다.
3. `World`는 방, 아이템, NPC 같은 고정 월드 데이터를 관리한다.
4. `Player`는 현재 위치와 인벤토리 같은 진행 상태를 관리한다.
5. `CommandParser`는 입력 문자열을 명령어 객체로 변환한다.
6. `LLMClient`는 설계만 해두고 실제 AI NPC 연결은 다음 단계로 미룬다.

## 생성하거나 수정한 파일

| 파일 | 작업 내용 |
|---|---|
| `docs/text-mud-cpp-architecture.md` | C++ MUD 게임 프로젝트 구조 설계 문서 생성 |
| `docs/work-logs/text-mud-cpp-architecture.md` | 이번 작업 기록 생성 |
| `README.md` | 문서 목록에 구조 설계 문서 추가 |

## 다음에 이어서 할 수 있는 작업

1. 이 설계 문서를 바탕으로 첫 번째 코드 구현 요청 프롬프트를 만든다.
2. `Item`, `NPC`, `Room`, `Player`, `World` 파일을 먼저 분리한다.
3. `CommandParser`와 `GameEngine`을 추가한다.
4. 기존 `main.cpp`의 첫 플레이 루프가 동일하게 동작하는지 확인한다.
5. 이후 `LLMClient`를 기존 로컬 LLM 채팅 코드와 연결한다.
