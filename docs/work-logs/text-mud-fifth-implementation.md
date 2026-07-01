# C++ MUD 게임 5차 Ollama 연동 구현 작업 기록

## 설명

이 문서는 C++ 텍스트 MUD 게임의 5차 작업 기록을 정리한다.

이번 작업은 사용자가 제공한 5차 작업 설계 문서와 코드 요청 프롬프트를 바탕으로, 4차에서 만든 `LLMClient` mock 구조를 실제 Ollama 로컬 LLM 호출 구조로 확장한 기록이다.

## 핵심 정리

1. `LLMClient`에 실제 Ollama 호출 기능을 추가했다.
2. 기본 모델을 `qwen3:8b`로 설정했다.
3. 게임 시작 시 warm-up 요청을 보내도록 했다.
4. 게임 종료 시 모델 unload 요청을 보내도록 했다.
5. Ollama 미실행 또는 모델 오류 시 `ask`만 fallback 응답을 출력하도록 했다.
6. 기존 `talk`, `use`, `save`, `load` 기능을 유지했다.
7. 5차 설계 문서를 추가했다.

## 사용자가 지시한 내용

사용자는 첨부한 5차 작업 문서를 읽고 다음 작업을 진행하라고 지시했다.

```text
5차 작업이야 해당 작업 문서화 및 작업까지 진행해줘
```

요구사항의 핵심은 다음과 같다.

```text
1. mock LLMClient를 실제 Ollama 호출 구조로 교체
2. qwen3:8b 모델 사용
3. Ollama 미실행 시 게임은 계속 실행
4. ask 명령어만 fallback 응답 처리
5. warm-up과 unload 생명주기 추가
6. <think> 태그 제거
7. 응답 길이 제한
```

## assistant가 작업한 내용

### 설계 문서 정리

다음 문서를 추가했다.

```text
docs/text-mud-fifth-implementation-design-and-prompt.md
```

문서에는 5차 구현 목표, 실제 적용 방식, `LLMClient` 생명주기, fallback 처리, 정상/실패 테스트 시나리오, 코드 요청 프롬프트를 정리했다.

### 외부 라이브러리 적용 방식 판단

첨부 설계 문서는 `cpp-httplib`와 `nlohmann/json` 사용을 제안했다.

하지만 현재 저장소에는 이미 `LocalAIChat` 프로젝트에 Winsock 기반 Ollama 호출 코드가 있고, 현재 작업 환경에서는 새 외부 헤더를 받지 않는 편이 안정적이다.

따라서 5차 구현은 새 `external/` 헤더를 추가하지 않고, 기존 프로젝트의 Ollama 호출 방식을 `TextMUDGame`에 맞게 적용했다.

### LLMClient 수정

`LLMClient`에 다음 기능을 추가했다.

```text
initialize()
generateResponse()
shutdown()
isAvailable()
isMockMode()
getModelName()
```

내부 처리 함수는 다음과 같다.

```text
generateMockResponse()
generateOllamaResponse()
postGenerate()
buildRequestBody()
sendHttpPost()
extractResponseText()
removeThinkTags()
trimResponseLength()
trimWhitespace()
```

### Ollama 요청 구조

Ollama API는 다음 주소로 호출한다.

```text
http://localhost:11434/api/generate
```

요청에는 다음 값을 포함한다.

```text
model = qwen3:8b
prompt = NPC 프롬프트
stream = false
keep_alive = 10m
```

종료 시 모델 unload 요청은 다음 형태로 보낸다.

```text
prompt = 빈 문자열
keep_alive = 0
```

### GameEngine 수정

`GameEngine` 생성자에서 `llmClient.initialize()`를 호출한다.

`GameEngine` 소멸자에서 `llmClient.shutdown()`을 호출한다.

시작 문구에는 AI NPC 기능 활성화 여부를 표시한다.

```text
[알림] AI NPC 기능이 활성화되었습니다.
```

또는:

```text
[알림] 로컬 LLM을 사용할 수 없어 AI NPC 질문 기능이 비활성화됩니다.
```

### Visual Studio 프로젝트 설정 수정

Winsock 사용을 위해 다음 라이브러리를 링크하도록 설정했다.

```text
ws2_32.lib
```

## assistant가 답변한 핵심 내용

5차 작업은 실제 Ollama 호출을 붙이되, Ollama가 필수 실행 조건이 되지 않도록 처리했다.

Ollama가 꺼져 있으면 `ask` 명령어는 다음 fallback 응답을 출력한다.

```text
경비병: 지금은 대답하기 어렵군.
상인: 지금은 대답하기 어렵군.
```

기본 MUD 기능인 `look`, `go`, `items`, `take`, `inventory`, `talk`, `use`, `save`, `load`, `help`, `quit`은 계속 정상 동작한다.

## 생성하거나 수정한 파일

```text
TextMUDGame/src/LLMClient.h
TextMUDGame/src/LLMClient.cpp
TextMUDGame/src/GameEngine.h
TextMUDGame/src/GameEngine.cpp
TextMUDGame/TextMUDGame.vcxproj
docs/text-mud-fifth-implementation-design-and-prompt.md
docs/work-logs/text-mud-fifth-implementation.md
README.md
```

## 검증 내용

직접 컴파일 방식으로 빌드를 확인했다.

```text
cl /EHsc /std:c++17 /utf-8 ... ws2_32.lib
```

확인한 실행 시나리오는 다음과 같다.

```text
go south
ask guard 성문은 어떻게 열 수 있나요?
go south
talk guard
quit
```

Ollama를 사용할 수 없는 상태에서 `ask`는 fallback 응답을 출력했고, 이후 잠긴 성문 이동은 차단되어 AI 응답이 게임 상태를 바꾸지 않음을 확인했다.

또한 다음 흐름을 확인했다.

```text
go east
take old_key
go west
go south
use old_key
go south
save
load
```

## 다음에 이어서 할 수 있는 작업

다음 6차 작업 후보는 다음과 같다.

```text
1. 설정 파일 분리
   - modelName
   - host
   - port
   - mockMode
   - keepAlive

2. NPC별 프롬프트 개선
   - 말투
   - 힌트 강도
   - 금지 정보

3. 데이터 파일 분리
   - Room
   - Item
   - NPC
```
