# C++ MUD 게임 4차 구현 설계 문서와 코드 요청 프롬프트

## 설명

이 문서는 C++ 텍스트 MUD 게임의 4차 구현 설계와 코드 요청 프롬프트를 정리한다.

4차 구현의 목표는 `ask` 명령어로 NPC에게 자연어 질문을 하고, NPC 정보와 현재 게임 상태를 바탕으로 AI NPC 응답 프롬프트를 만드는 구조를 추가하는 것이다.

이번 단계에서는 실제 Ollama HTTP API 호출을 구현하지 않는다. `LLMClient`는 mock 응답을 반환하며, 실제 로컬 LLM 연동은 5차 작업으로 분리한다.

## 핵심 정리

1. `ask NPC_ID 질문` 명령어를 추가한다.
2. `talk`는 고정 대사, `ask`는 AI NPC 응답으로 역할을 분리한다.
3. `NPC`에 AI 사용 여부, 성격, 지식 정보를 추가한다.
4. `LLMClient`는 mock 응답을 반환한다.
5. `GameEngine`은 NPC 프롬프트를 생성하고 `LLMClient`에 전달한다.
6. AI NPC는 대답만 하며 게임 상태를 직접 변경하지 않는다.
7. `use old_key`, `save`, `load`, 기존 `talk` 동작은 유지한다.

## 추가 명령어

```text
ask npcId 질문내용
```

예시:

```text
ask guard 성 안에는 뭐가 있나요?
ask merchant 이 열쇠는 어디에 쓰나요?
```

역할 구분:

```text
talk guard
→ 고정 대사 출력

ask guard 성 안에는 뭐가 있나요?
→ AI NPC 응답 출력
```

## 구현 범위

```text
1. ask 명령어 처리
2. NPC AI 응답 가능 여부 추가
3. NPC 성격 정보 추가
4. NPC 지식 정보 추가
5. mock LLMClient 응답 구현
6. NPC 프롬프트 생성 함수 추가
7. 현재 게임 상태를 프롬프트에 포함
8. AI NPC 응답 규칙을 프롬프트에 포함
9. ask 명령어 실패 상황 처리
10. 기존 talk 명령어 유지
```

## 제외 범위

```text
1. 실제 Ollama HTTP API 호출
2. libcurl 또는 cpp-httplib 연동
3. system curl 호출
4. 스트리밍 응답
5. AI 응답에 따른 게임 상태 변경
6. AI가 아이템 지급
7. AI가 문 열기
8. 대화 기록 저장
9. AI 응답 save/load
10. 온라인 API 연동
```

## 핵심 설계 원칙

```text
AI NPC는 대답만 한다.
게임 상태 변경은 반드시 C++ 게임 로직이 담당한다.
```

따라서 AI 응답이 어떤 문장을 반환하더라도 실제 게임 상태는 바뀌면 안 된다.

성문은 오직 다음 명령어로만 열린다.

```text
use old_key
```

아이템 획득은 오직 다음 명령어로만 처리된다.

```text
take itemId
```

## NPC 데이터 확장

기존 NPC 정보:

```text
id
name
fixedDialogue
```

4차에서 추가되는 정보:

```text
aiEnabled
personality
knowledge
```

### 경비병

```text
ID: guard
이름: 경비병
AI 사용 가능: true
성격: 신중하고 규칙을 중요하게 생각한다.
지식:
- 성문은 낡은 열쇠가 있어야 열 수 있다.
- 성문 너머에는 오래된 성 안뜰이 있다.
- 성문은 함부로 열 수 없다.
```

### 상인

```text
ID: merchant
이름: 상인
AI 사용 가능: true
성격: 친절하지만 장사꾼답게 약간 계산적이다.
지식:
- 잡화점에는 오래된 물건이 많다.
- 낡은 열쇠는 성문과 관련이 있을 수 있다.
- 마을 사람들은 성 안쪽을 조심스러워한다.
```

## 프롬프트 구성

`GameEngine::buildNPCPrompt()`는 다음 정보를 포함한다.

```text
1. NPC 이름
2. NPC 성격
3. NPC가 알고 있는 정보
4. 현재 방 ID
5. 현재 방 이름
6. 현재 방 설명
7. 플레이어 인벤토리
8. castleGateUnlocked 상태
9. reachedCourtyard 상태
10. 월드 기본 정보
11. AI 응답 규칙
12. 플레이어 질문
```

프롬프트에는 반드시 다음 규칙을 넣는다.

```text
1. NPC의 입장에서만 대답한다.
2. 게임에 없는 장소, 아이템, NPC를 새로 만들지 않는다.
3. 플레이어에게 직접 명령어를 실행해주지 않는다.
4. 게임 상태를 변경했다고 말하지 않는다.
5. 아이템을 주거나 문을 열어주지 않는다.
6. 답변은 3문장 이내로 한다.
7. 힌트는 줄 수 있지만 정답을 너무 직접적으로 말하지 않는다.
```

## 실패 메시지

```text
ask
→ 누구에게 물어볼지 입력해라.

ask guard
→ 무엇을 물어볼지 입력해라.

현재 방에 없는 NPC에게 질문
→ 이곳에는 그런 NPC가 없다.

AI 응답이 비활성화된 NPC에게 질문
→ 이 NPC는 질문에 답할 수 없다.

LLM 응답 실패
→ 경비병: 지금은 대답하기 어렵군.
```

## 코드 요청 프롬프트

```text
C++17 콘솔 텍스트 MUD 게임에 4차 기능을 구현해줘.

목표는 ask NPC_ID 질문 명령어를 추가하고, NPC 정보와 현재 게임 상태를 바탕으로 AI NPC 응답 프롬프트를 만드는 것이다.

이번 단계에서는 실제 Ollama HTTP API 호출을 구현하지 말고 mock LLMClient를 사용해라.

구현 요구사항:
1. Command에 target 문자열을 추가해 두 번째 단어 이후 문장을 보존한다.
2. GameEngine::handleAsk()를 추가한다.
3. GameEngine::buildNPCPrompt()를 추가한다.
4. NPC에 aiEnabled, personality, knowledge를 추가한다.
5. LLMClient::generateResponse()는 mock 응답을 반환한다.
6. talk 명령어는 기존 고정 대사를 유지한다.
7. ask 응답은 게임 상태를 변경하지 않는다.
8. use old_key, save, load는 기존처럼 동작해야 한다.

정상 테스트:
go south
ask guard 성 안에는 뭐가 있나요?

go east
ask merchant 이 열쇠는 어디에 쓰나요?

실패 테스트:
ask
ask guard
ask dragon 성 안에는 뭐가 있나요?
```

## 다음 작업

5차 작업에서는 `LLMClient` 내부 mock 구현을 실제 Ollama 로컬 LLM 호출로 교체한다.
