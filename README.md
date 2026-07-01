# AI-Learn

## 설명

이 프로젝트는 C++ 학습을 돕는 로컬 AI 채팅 assistant를 준비하기 위한 저장소이다.

문서는 기본적으로 한글로 작성하며, 기술적 정확성이나 API 이름 유지가 필요한 경우에만 영어를 사용한다.

## 실행 준비

로컬 LLM은 Ollama의 `qwen3:8b` 모델을 사용한다.

실행 전에 Ollama 서버와 모델을 준비한다.

```text
ollama serve
ollama pull qwen3:8b
```

기본 모델은 `qwen3:8b`이다.

## Visual Studio 실행 방법

이 프로젝트는 CMake를 사용하지 않고 Visual Studio 솔루션 파일을 기준으로 빌드한다.

로컬 LLM 채팅 프로그램은 Visual Studio에서 다음 파일을 연다.

```text
LocalAIChat\LocalAIChat.sln
```

텍스트 MUD 게임은 별도 솔루션으로 작업한다.

```text
TextMUDGame\TextMUDGame.sln
```

권장 구성은 다음과 같다.

```text
Debug | x64
```

또는 배포용 확인이 필요하면 다음 구성을 사용할 수 있다.

```text
Release | x64
```

`LocalAIChat` 프로젝트를 시작 프로젝트로 실행하면 콘솔에서 채팅을 입력하고 Ollama `qwen3:8b` 모델로 응답을 생성한다.

프로그램 실행 시 Ollama 서버가 꺼져 있으면 자동으로 시작한다.

프로그램을 종료하면 실행 중인 `ollama.exe` 서버 프로세스도 함께 종료한다.

`TextMUDGame.sln`을 열어 실행하면 MUD 게임용 콘솔 프로그램이 실행된다. 현재 5차 버전은 `낡은 성문의 비밀` 시나리오를 기준으로 `look`, `go`, `items`, `take`, `use`, `inventory`, `talk`, `ask`, `save`, `load`, `help`, `quit` 명령어를 처리한다.

5차 버전의 `ask` 명령어는 `LLMClient`를 통해 로컬 Ollama `qwen3:8b` 모델 호출을 시도한다. Ollama 서버나 모델을 사용할 수 없는 경우에도 게임은 종료되지 않으며, AI NPC 질문만 fallback 응답으로 처리된다.

## 문서 목록

| 문서 | 설명 |
|---|---|
| [AGENTS.md](AGENTS.md) | assistant 응답 지침 |
| [docs/project-conventions.md](docs/project-conventions.md) | 프로젝트 코딩 컨벤션 |
| [docs/commit-conventions.md](docs/commit-conventions.md) | 커밋 메시지 컨벤션 |
| [docs/assistant-workflow.md](docs/assistant-workflow.md) | assistant 작업 및 문서화 운영 규칙 |
| [docs/assistant-response-notes.md](docs/assistant-response-notes.md) | assistant 답변 정리 기준 |
| [docs/text-mud-game-plan.md](docs/text-mud-game-plan.md) | C++ 로컬 AI 기반 텍스트 MUD 게임 기획 |
| [docs/text-mud-first-play-loop.md](docs/text-mud-first-play-loop.md) | 텍스트 MUD 첫 번째 플레이 루프와 시나리오 |
| [docs/text-mud-cpp-architecture.md](docs/text-mud-cpp-architecture.md) | C++ MUD 게임 프로젝트 구조 설계 |
| [docs/text-mud-first-implementation-design-and-prompt.md](docs/text-mud-first-implementation-design-and-prompt.md) | C++ MUD 게임 1차 구현 설계 문서와 코드 요청 프롬프트 |
| [docs/text-mud-second-implementation-design-and-prompt.md](docs/text-mud-second-implementation-design-and-prompt.md) | C++ MUD 게임 2차 구현 설계 문서와 코드 요청 프롬프트 |
| [docs/text-mud-third-implementation-design-and-prompt.md](docs/text-mud-third-implementation-design-and-prompt.md) | C++ MUD 게임 3차 구현 설계 문서와 코드 요청 프롬프트 |
| [docs/text-mud-fourth-implementation-design-and-prompt.md](docs/text-mud-fourth-implementation-design-and-prompt.md) | C++ MUD 게임 4차 AI NPC 응답 구조 설계 문서와 코드 요청 프롬프트 |
| [docs/text-mud-fifth-implementation-design-and-prompt.md](docs/text-mud-fifth-implementation-design-and-prompt.md) | C++ MUD 게임 5차 Ollama 연동 설계 문서와 코드 요청 프롬프트 |
| [PROMPT_REVIEW.md](PROMPT_REVIEW.md) | AI 코드 요청 프롬프트 검토 기준 |
| [docs/work-logs/](docs/work-logs/) | 개별 작업 기록 |

## 다음 학습 추천

다음에는 C++ 학습 순서를 정리하는 `docs/cpp-learning-roadmap.md` 문서를 추가하는 것을 추천한다.
