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

Visual Studio에서 다음 파일을 연다.

```text
LocalAIChat\LocalAIChat.sln
```

권장 구성은 다음과 같다.

```text
Debug | x64
```

또는 배포용 확인이 필요하면 다음 구성을 사용할 수 있다.

```text
Release | x64
```

빌드 후 콘솔에서 채팅을 입력하면 Ollama `qwen3:8b` 모델로 응답을 생성한다.

프로그램 실행 시 Ollama 서버가 꺼져 있으면 자동으로 시작한다.

프로그램을 종료하면 실행 중인 `ollama.exe` 서버 프로세스도 함께 종료한다.

## 문서 목록

| 문서 | 설명 |
|---|---|
| [AGENTS.md](AGENTS.md) | assistant 응답 지침 |
| [docs/project-conventions.md](docs/project-conventions.md) | 프로젝트 코딩 컨벤션 |
| [docs/commit-conventions.md](docs/commit-conventions.md) | 커밋 메시지 컨벤션 |
| [docs/assistant-workflow.md](docs/assistant-workflow.md) | assistant 작업 및 문서화 운영 규칙 |
| [docs/assistant-response-notes.md](docs/assistant-response-notes.md) | assistant 답변 정리 기준 |
| [PROMPT_REVIEW.md](PROMPT_REVIEW.md) | AI 코드 요청 프롬프트 검토 기준 |
| [docs/work-logs/](docs/work-logs/) | 개별 작업 기록 |

## 다음 학습 추천

다음에는 C++ 학습 순서를 정리하는 `docs/cpp-learning-roadmap.md` 문서를 추가하는 것을 추천한다.
