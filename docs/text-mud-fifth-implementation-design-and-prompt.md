# C++ MUD 게임 5차 구현 설계 문서와 코드 요청 프롬프트

## 설명

이 문서는 C++ 텍스트 MUD 게임의 5차 구현 설계와 코드 요청 프롬프트를 정리한다.

5차 구현의 목표는 4차에서 만든 `LLMClient` mock 구조를 실제 Ollama 로컬 LLM 호출 방식으로 교체하는 것이다.

첨부 설계 문서에서는 `cpp-httplib`와 `nlohmann/json` 헤더 사용을 제안했지만, 현재 저장소에는 이미 `LocalAIChat` 프로젝트에 Winsock 기반 Ollama 호출 코드가 있다. 따라서 이번 구현은 새 외부 헤더를 추가하지 않고 기존 프로젝트 방식과 같은 Windows Winsock HTTP POST 방식으로 적용한다.

## 핵심 정리

1. `LLMClient`가 `localhost:11434/api/generate`로 Ollama API를 호출한다.
2. 기본 모델 이름은 `qwen3:8b`이다.
3. `mockMode` 구조는 유지한다.
4. 게임 시작 시 모델 warm-up 요청을 보낸다.
5. 게임 종료 시 모델 unload 요청을 보낸다.
6. Ollama를 사용할 수 없어도 기본 MUD 기능은 계속 실행된다.
7. Ollama 호출 실패 시 `ask` 명령어만 fallback 응답을 출력한다.
8. `<think>...</think>` 구간을 제거한다.
9. 응답 앞뒤 공백을 제거한다.
10. 응답 길이를 500자로 제한한다.
11. AI 응답은 게임 상태를 변경하지 않는다.

## 5차 구현 범위

```text
1. LLMClient 생명주기 추가
   - initialize()
   - generateResponse()
   - shutdown()

2. Ollama HTTP API 호출
   - host: localhost
   - port: 11434
   - path: /api/generate

3. 요청 JSON 생성
   - model
   - prompt
   - stream: false
   - keep_alive

4. 응답 처리
   - response 필드 추출
   - JSON escape 해제
   - <think> 태그 제거
   - 공백 제거
   - 길이 제한

5. 실패 처리
   - 서버 미실행
   - 모델 미설치
   - HTTP 응답 오류
   - 응답 필드 없음
```

## 제외 범위

```text
1. 스트리밍 응답
2. 대화 기록 저장
3. NPC별 장기 기억
4. AI 응답을 save/load에 포함
5. AI가 아이템 지급
6. AI가 문 열기
7. AI가 퀘스트 상태 변경
8. 여러 모델 선택 UI
9. 설정 파일 분리
10. 온라인 API 연동
11. 멀티플레이 AI 대화
12. Ollama 서버 프로세스 강제 종료
```

## 핵심 설계 원칙

```text
AI NPC는 대답만 한다.
게임 상태 변경은 반드시 C++ 게임 로직이 담당한다.
```

따라서 AI가 다음과 같이 말하더라도 실제 성문 상태는 바뀌지 않는다.

```text
경비병: 내가 성문을 열어주겠다.
```

성문은 오직 다음 명령어로만 열린다.

```text
use old_key
```

## LLMClient 구조

```cpp
// 5차 LLMClient 핵심 구조
class LLMClient
{
public:
    LLMClient();
    LLMClient(std::string host, int port, std::string modelName, bool mockMode);

    bool initialize();
    std::string generateResponse(const std::string& prompt) const;
    void shutdown();

    bool isAvailable() const;
    bool isMockMode() const;
    const std::string& getModelName() const;
};
```

기본 설정값:

```text
mockMode = false
available = false
host = localhost
port = 11434
modelName = qwen3:8b
keepAlive = 10m
readTimeoutSeconds = 60
writeTimeoutSeconds = 5
maxResponseLength = 500
```

## GameEngine 생명주기

현재 프로젝트는 `GameEngine::run()`이 없고, `main.cpp`가 입력 루프를 담당한다.

따라서 `LLMClient` 생명주기는 다음 방식으로 연결한다.

```text
GameEngine 생성자
→ llmClient.initialize()

GameEngine 소멸자
→ llmClient.shutdown()
```

게임 시작 안내:

```text
[알림] AI NPC 기능이 활성화되었습니다.
```

또는:

```text
[알림] 로컬 LLM을 사용할 수 없어 AI NPC 질문 기능이 비활성화됩니다.
```

## fallback 응답

Ollama 호출이 실패하거나 응답이 비어 있으면 `GameEngine::handleAsk()`가 fallback 대사를 출력한다.

```text
경비병: 지금은 대답하기 어렵군.
상인: 지금은 대답하기 어렵군.
```

## 정상 테스트 시나리오

Ollama 서버와 `qwen3:8b` 모델이 준비된 상태:

```text
go south
ask guard 성문은 어떻게 열 수 있나요?
```

예상 흐름:

```text
1. GameEngine이 NPC 프롬프트를 생성한다.
2. LLMClient가 Ollama API를 호출한다.
3. response 필드를 추출한다.
4. 응답 후처리를 수행한다.
5. NPC 이름을 붙여 출력한다.
```

## 실패 테스트 시나리오

Ollama 서버가 꺼져 있는 상태:

```text
go south
ask guard 성문은 어떻게 열 수 있나요?
```

예상 출력:

```text
경비병: 지금은 대답하기 어렵군.
```

기본 MUD 기능은 계속 동작해야 한다.

```text
look
go
items
take
inventory
talk
use
save
load
help
quit
```

## 코드 요청 프롬프트

```text
C++17 콘솔 텍스트 MUD 게임의 5차 기능을 구현해줘.

목표는 4차에서 만든 mock LLMClient를 실제 Ollama 로컬 LLM 호출 방식으로 확장하는 것이다.

요구사항:
1. LLMClient에 initialize(), generateResponse(), shutdown()을 추가한다.
2. 기본 모델은 qwen3:8b로 설정한다.
3. localhost:11434/api/generate에 HTTP POST 요청을 보낸다.
4. 요청에는 model, prompt, stream:false, keep_alive를 포함한다.
5. 응답 JSON의 response 필드를 추출한다.
6. <think>...</think> 구간을 제거한다.
7. 응답 앞뒤 공백을 제거한다.
8. 응답 길이를 500자로 제한한다.
9. Ollama 호출 실패 시 빈 문자열을 반환한다.
10. GameEngine은 빈 응답이면 NPC별 fallback 대사를 출력한다.
11. 게임 시작 시 warm-up 요청을 보낸다.
12. 게임 종료 시 모델 unload 요청을 보낸다.
13. Ollama 서버 자체를 종료하지 않는다.
14. talk, use, save, load 기능은 기존처럼 유지한다.
15. AI 응답은 게임 상태를 변경하지 않는다.

현재 저장소에 기존 Ollama 호출 코드가 있다면 그 방식을 우선 재사용해도 된다.
```

## 다음 작업

6차 작업에서는 설정 파일 분리 또는 NPC별 프롬프트 개선을 진행할 수 있다.
