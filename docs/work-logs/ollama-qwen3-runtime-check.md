# Ollama qwen3:8b 실행 확인 작업 기록

## 설명

사용자는 `Could not connect to Ollama. Make sure Ollama is running on localhost:11434.` 오류가 나온다고 알려주었고, 현재 사용하는 로컬 LLM인 `qwen3:8b`를 사용해 달라고 지시했다.

이에 따라 Ollama 서버 실행 상태, 설치된 모델, LocalAIChat 실행 결과를 확인했다.

## 사용자 지시 내용

1. Ollama 연결 오류를 확인한다.
2. 로컬 LLM `qwen3:8b`를 사용한다.
3. 실제 실행이 되도록 처리한다.

## assistant가 작업한 내용

1. `127.0.0.1:11434` 포트가 닫혀 있어 Ollama 서버가 실행 중이 아님을 확인했다.
2. 로컬에 설치된 Ollama 실행 파일을 확인했다.
3. Ollama 서버를 백그라운드에서 실행했다.
4. `ollama list` 결과에 `qwen3:8b` 모델이 있음을 확인했다.
5. LocalAIChat을 실행해 `qwen3:8b` 응답을 받는지 확인했다.
6. Ollama JSON 응답의 `\u003c` 같은 Unicode escape가 정상 문자로 표시되도록 디코딩 코드를 수정했다.
7. `Debug | x64` 구성을 다시 빌드했다.
8. 실행 출력에서 `<iostream>` 같은 문자가 정상 표시되는지 확인했다.
9. README에 Ollama 서버 실행과 기본 모델 안내를 보강했다.

## assistant가 답변한 핵심 내용

`qwen3:8b` 모델은 설치되어 있었지만 Ollama 서버가 실행 중이지 않아 연결 오류가 발생했다.

Ollama 서버를 실행한 뒤 LocalAIChat은 `qwen3:8b`로 정상 응답을 받았다.

## 생성하거나 수정한 파일

| 파일 | 내용 |
|---|---|
| `LocalAIChat/src/OllamaClient.cpp` | JSON Unicode escape 디코딩 보강 |
| `README.md` | Ollama 실행과 기본 모델 안내 보강 |
| `docs/work-logs/ollama-qwen3-runtime-check.md` | 이번 작업 기록 |

## 확인 결과

설치된 모델 목록에서 다음 모델을 확인했다.

```text
qwen3:8b
```

다음 구성으로 빌드 확인을 완료했다.

```text
Configuration=Debug
Platform=x64
```

MSBuild 결과는 경고 0개, 오류 0개이다.

LocalAIChat 실행 결과, `qwen3:8b`가 C++ 질문에 응답하는 것을 확인했다.

## 다음에 이어서 할 수 있는 작업

1. 앱 시작 시 Ollama 서버 연결 상태를 먼저 검사하고 더 친절한 안내를 출력한다.
2. Ollama 서버 자동 실행 기능을 C++ 코드에 추가할지 결정한다.
3. 모델 이름을 설정 파일로 분리해 Visual Studio에서 쉽게 바꿀 수 있게 만든다.

## 핵심 정리

- 연결 오류의 원인은 모델 부재가 아니라 Ollama 서버 미실행이었다.
- `qwen3:8b` 모델은 설치되어 있었다.
- Ollama 서버 실행 후 LocalAIChat은 정상 응답을 받았다.
- JSON Unicode escape 디코딩 문제도 함께 수정했다.
