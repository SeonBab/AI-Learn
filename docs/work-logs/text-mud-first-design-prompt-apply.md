# MUD 1차 설계 문서와 프롬프트 적용 작업 기록

## 설명

사용자는 첨부한 MUD 1차 구현 설계 문서와 코드 요청 프롬프트를 읽고 프로젝트에 적용해달라고 요청했다.

이번 작업에서는 첨부 내용을 기준 문서로 정리하고, 현재 `TextMUDGame`의 1차 시나리오를 `낡은 성문의 비밀`로 맞췄다.

## 핵심 정리

## 사용자가 지시한 내용

1. 첨부된 설계 문서와 코드 요청 프롬프트를 읽는다.
2. 현재 프로젝트에 필요한 내용을 적용한다.
3. 기준은 C++ 학습용 로컬 AI 채팅 프로그램 확장과 텍스트 기반 MUD 게임 제작이다.

## assistant가 작업한 내용

1. 첨부된 설계 문서와 코드 요청 프롬프트를 확인했다.
2. `docs/text-mud-first-implementation-design-and-prompt.md` 문서를 생성했다.
3. `TextMUDGame`의 월드 데이터를 `마을 광장`, `잡화점`, `성문 앞` 구성으로 변경했다.
4. 아이템을 `herb`에서 `old_key`로 변경했다.
5. NPC를 `merchant`, `guard` 기준으로 변경했다.
6. 시작 방을 `village_square`로 변경했다.
7. 게임 제목을 `낡은 성문의 비밀`로 변경했다.
8. 1차 구현 범위에서 제외된 `give` 명령 처리를 제거했다.
9. 기존 구조 설계 문서에 남아 있던 `herb`, `villager`, `village_entrance` 예시를 1차 기준으로 수정했다.
10. README 문서 목록과 실행 설명을 갱신했다.
11. 1차 완료 기준 플레이 흐름을 컴파일 후 실행해 검증했다.

## assistant가 답변한 핵심 내용

1. 첨부 문서의 1차 구현 기준은 AI 기능 없는 기본 MUD 엔진이다.
2. 1차 완료 기준은 `look -> go east -> take old_key -> talk merchant -> go south -> talk guard -> quit` 흐름이다.
3. 로컬 LLM, 저장, 전투, 퀘스트 시스템은 다음 단계로 미룬다.

## 생성하거나 수정한 파일

| 파일 | 작업 내용 |
|---|---|
| `docs/text-mud-first-implementation-design-and-prompt.md` | 첨부 설계 문서와 코드 요청 프롬프트 정리 |
| `TextMUDGame/src/World.cpp` | 1차 월드 데이터를 `낡은 성문의 비밀` 시나리오로 변경 |
| `TextMUDGame/src/GameEngine.cpp` | 시작 방, 제목, 안내 문구와 예시 명령어 변경 |
| `TextMUDGame/src/CommandParser.h` / `CommandParser.cpp` | 1차 범위에 없는 `give` 명령 제거 |
| `docs/text-mud-cpp-architecture.md` | 1차 구현 기준과 충돌하는 이전 시나리오 예시 수정 |
| `README.md` | 새 기준 문서 링크 추가 |
| `docs/work-logs/text-mud-first-design-prompt-apply.md` | 이번 작업 기록 생성 |

## 검증 결과

Visual Studio Developer Command 환경에서 `cl.exe`로 컴파일을 확인했다.

완료 기준 플레이 흐름도 실행했다.

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

확인 결과 낡은 열쇠 획득, 인벤토리 확인, 상인 대화, 경비병 대화, 도움말 출력, 종료가 정상 동작했다.

## 다음에 이어서 할 수 있는 작업

1. 2차 작업으로 `use old_key`와 잠긴 성문 열기를 설계한다.
2. `save / load` 기능을 위한 저장 데이터 구조를 설계한다.
3. `ask guard 질문` 명령어와 `LLMClient` 연동 범위를 설계한다.
