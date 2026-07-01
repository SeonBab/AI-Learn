# C++ MUD 게임 4차 AI NPC 응답 구조 구현 작업 기록

## 설명

이 문서는 C++ 텍스트 MUD 게임의 4차 작업 기록을 정리한다.

이번 작업은 사용자가 제공한 4차 작업 설계 문서와 코드 요청 프롬프트를 바탕으로 `ask` 명령어와 mock AI NPC 응답 구조를 `TextMUDGame` 프로젝트에 적용한 기록이다.

## 핵심 정리

1. `ask NPC_ID 질문` 명령어를 추가했다.
2. `talk`는 고정 대사, `ask`는 AI NPC 응답으로 역할을 분리했다.
3. `NPC`에 AI 사용 여부, 성격, 지식 정보를 추가했다.
4. `LLMClient`에 mock 응답 생성 함수를 추가했다.
5. `GameEngine`에 NPC 프롬프트 생성 흐름을 추가했다.
6. AI 응답은 게임 상태를 변경하지 않도록 처리했다.
7. 4차 설계 문서를 `docs/`에 추가했다.

## 사용자가 지시한 내용

사용자는 첨부한 4차 작업 문서를 읽고 다음 작업을 진행하라고 지시했다.

```text
이건 4차 작업이야 해당 문서들 정리해주고 작업까지 바로 진행해줘
```

요구사항의 핵심은 다음과 같다.

```text
1. ask 명령어 추가
2. NPC AI 응답 구조 추가
3. NPC 성격과 지식 정보 추가
4. mock LLMClient 사용
5. NPC 프롬프트 생성
6. 실제 Ollama 연동은 5차 작업으로 분리
```

## assistant가 작업한 내용

### 설계 문서 정리

다음 문서를 추가했다.

```text
docs/text-mud-fourth-implementation-design-and-prompt.md
```

문서에는 4차 구현 목표, 제외 범위, 명령어 형식, NPC 데이터 확장, 프롬프트 구성, 실패 메시지, 코드 요청 프롬프트를 정리했다.

### CommandParser 수정

`Command` 구조체에 `target` 문자열을 추가했다.

이를 통해 `ask guard 성 안에는 뭐가 있나요?` 입력에서 다음 값을 보존할 수 있다.

```text
target = guard 성 안에는 뭐가 있나요?
```

기존 `arguments`는 유지했기 때문에 `go`, `take`, `use`, `talk` 같은 기존 명령어 흐름은 계속 사용할 수 있다.

### NPC 수정

`NPC`에 다음 정보를 추가했다.

```text
aiEnabled
personality
knowledge
```

상인과 경비병에게 각각 AI 응답 가능 여부, 성격, 지식을 설정했다.

### LLMClient 수정

`LLMClient::generateResponse()`를 추가했다.

4차에서는 실제 LLM 호출 대신 다음 mock 응답을 반환한다.

```text
그 질문에 대해선 조심스럽게 생각해보는 게 좋겠군.
```

### GameEngine 수정

`GameEngine`에 다음 기능을 추가했다.

```text
handleAsk()
buildNPCPrompt()
formatInventoryForPrompt()
LLMClient llmClient_
```

`ask` 명령어 처리 흐름은 다음과 같다.

```text
1. 질문 대상 NPC ID 확인
2. 질문 내용 확인
3. 현재 방에 NPC가 있는지 확인
4. NPC가 AI 응답 가능한지 확인
5. NPC 정보와 현재 게임 상태로 프롬프트 생성
6. LLMClient mock 응답 생성
7. NPC 이름을 붙여 출력
```

## assistant가 답변한 핵심 내용

이번 4차 작업은 실제 Ollama 연동이 아니라 AI NPC 응답 구조를 먼저 만드는 단계로 진행했다.

AI NPC는 게임 상태를 직접 변경하지 않고, 대답 문자열만 반환한다. 성문 열기와 아이템 획득은 기존 C++ 게임 로직인 `use old_key`, `take itemId`로만 처리된다.

## 생성하거나 수정한 파일

```text
TextMUDGame/src/CommandParser.h
TextMUDGame/src/CommandParser.cpp
TextMUDGame/src/NPC.h
TextMUDGame/src/NPC.cpp
TextMUDGame/src/LLMClient.h
TextMUDGame/src/LLMClient.cpp
TextMUDGame/src/World.cpp
TextMUDGame/src/GameEngine.h
TextMUDGame/src/GameEngine.cpp
docs/text-mud-fourth-implementation-design-and-prompt.md
docs/work-logs/text-mud-fourth-implementation.md
README.md
```

## 다음에 이어서 할 수 있는 작업

다음 5차 작업에서는 `LLMClient`의 mock 응답을 실제 Ollama HTTP API 호출로 교체할 수 있다.

추천 작업 범위는 다음과 같다.

```text
1. Ollama 서버 실행 여부 확인
2. 모델 이름 설정
3. http://localhost:11434/api/generate 호출
4. JSON 요청 생성
5. JSON 응답 파싱
6. 호출 실패 시 fallback 메시지 반환
7. ask 명령어 결과를 실제 로컬 LLM 응답으로 출력
```
