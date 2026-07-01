# C++ 로컬 AI 채팅 프로그램 Ollama 연동 작업 기록

## 설명

사용자는 C++ 기반 로컬 AI 채팅 프로그램을 만들면서 C++ 문법, 프로그램 구조, 자료구조, 메모리, 알고리즘 기초를 학습하는 단계형 프로젝트를 진행하라고 지시했다.

추가 조건으로 로컬 LLM은 Ollama의 `qwen3:8b` 모델을 준비했다고 했고, C++ 프로그램에서 이 모델을 연동하도록 요청했다.

이번 작업에서는 첫 실행 가능한 구조를 만들기 위해 다음 파일을 추가했다.

```text
CMakeLists.txt
src/main.cpp
src/ChatApp.h
src/ChatApp.cpp
src/ChatMessage.h
src/ChatMessage.cpp
src/AIEngine.h
src/AIEngine.cpp
src/OllamaClient.h
src/OllamaClient.cpp
docs/work-logs/cpp-local-ai-chat-ollama.md
```

구현한 주요 내용은 다음과 같다.

1. `ChatApp` 클래스로 콘솔 채팅 루프를 분리했다.
2. `ChatMessage` 클래스로 사용자와 AI 메시지 데이터를 저장한다.
3. `AIEngine` 클래스가 사용자 입력을 학습용 프롬프트로 변환한다.
4. `OllamaClient` 클래스가 Ollama HTTP API의 `/api/generate` 엔드포인트를 호출한다.
5. 기본 모델은 `qwen3:8b`이며, `OLLAMA_MODEL` 환경 변수로 바꿀 수 있다.
6. `/help`, `/history`, `/clear`, `/exit` 명령어를 추가했다.

실행 전 준비 사항은 다음과 같다.

```text
ollama serve
ollama pull qwen3:8b
```

Windows에서 CMake와 C++ 컴파일러가 준비되어 있다면 다음 명령으로 빌드할 수 있다.

```text
cmake -S . -B build
cmake --build build
.\build\Debug\LocalAIChat.exe
```

현재 작업 환경의 PATH에서는 `cmake`, `cl`, `g++`, `ollama`가 바로 확인되지 않아 실제 빌드와 Ollama 호출 실행 검증은 완료하지 못했다.

## 핵심 정리

- C++ 콘솔 채팅 프로그램의 기본 구조를 `main`, `ChatApp`, `AIEngine`, `OllamaClient`, `ChatMessage`로 분리했다.
- Ollama `qwen3:8b`는 `http://127.0.0.1:11434/api/generate`로 호출한다.
- 외부 C++ HTTP 라이브러리를 추가하지 않고 Windows WinSock 기반의 간단한 HTTP POST 클라이언트를 작성했다.
- 대화 기록은 현재 메모리의 `std::vector<FChatMessage>`에 저장된다.
- 다음 작업으로 파일 저장과 불러오기, 최근 N개 대화 보기, 검색과 통계 기능을 확장할 수 있다.
