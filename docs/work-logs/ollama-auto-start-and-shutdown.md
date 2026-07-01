# Ollama 서버 자동 시작과 종료 작업 기록

## 설명

사용자는 `LocalAIChat` 실행 후 첫 질문에서 Ollama 연결 오류가 발생하는 동작이 정상적이지 않다고 알려주었다.

원인은 이전 작업에서 앱 종료 시 Ollama 서버를 함께 종료하도록 만들었지만, 다음 실행 때 Ollama 서버를 자동으로 다시 시작하는 기능이 없었기 때문이다.

이에 따라 `LocalAIChat` 시작 시 Ollama 서버 연결 상태를 확인하고, 서버가 꺼져 있으면 자동으로 실행한 뒤 준비될 때까지 기다리도록 수정했다.

## 사용자 지시 내용

1. 실행 직후 `Could not connect to Ollama` 오류가 나는 문제를 해결한다.
2. `qwen3:8b`를 사용하는 채팅 앱이 정상적으로 동작하게 한다.

## assistant가 작업한 내용

1. 앱 시작 시 `127.0.0.1:11434` 연결 여부를 확인하도록 했다.
2. Ollama 서버가 꺼져 있으면 `%LOCALAPPDATA%\Programs\Ollama\ollama.exe serve`를 백그라운드로 실행하도록 했다.
3. 서버 시작 후 최대 약 15초 동안 준비 상태를 기다리도록 했다.
4. 서버가 준비되면 기존 흐름대로 `qwen3:8b` 모델에 요청하도록 했다.
5. 앱 종료 시 기존처럼 `ollama.exe` 서버 프로세스를 종료하도록 유지했다.
6. `Debug | x64` 구성을 MSBuild로 빌드해 성공 여부를 확인했다.
7. Ollama 서버가 꺼진 상태에서 앱이 자동 시작, 응답 생성, 종료 시 서버 종료까지 수행하는지 확인했다.
8. README에 실행 시 자동 시작과 종료 시 자동 종료 동작을 정리했다.

## assistant가 답변한 핵심 내용

이제 사용자가 Visual Studio에서 `LocalAIChat`을 실행하면 Ollama 서버가 꺼져 있어도 앱이 자동으로 서버를 시작한다.

앱 종료 시에는 Ollama 서버도 함께 종료한다.

## 생성하거나 수정한 파일

| 파일 | 내용 |
|---|---|
| `LocalAIChat/src/main.cpp` | Ollama 서버 자동 시작과 준비 대기 기능 추가 |
| `README.md` | Ollama 자동 시작과 자동 종료 안내 추가 |
| `docs/work-logs/ollama-auto-start-and-shutdown.md` | 이번 작업 기록 |

## 확인 결과

다음 구성으로 빌드 확인을 완료했다.

```text
Configuration=Debug
Platform=x64
```

MSBuild 결과는 경고 0개, 오류 0개이다.

Ollama 서버가 꺼진 상태에서 실행했을 때 다음 흐름을 확인했다.

```text
Ollama 서버를 시작합니다.
Ollama 서버가 준비되었습니다.
AI: ...
Ollama 서버를 종료했습니다.
```

종료 후 `ollama` 프로세스가 남아 있지 않은 것도 확인했다.

## 다음에 이어서 할 수 있는 작업

1. Ollama 설치 경로를 찾지 못했을 때 더 자세한 안내를 출력한다.
2. 서버 자동 종료 여부를 설정으로 켜고 끌 수 있게 만든다.
3. 모델 준비 여부를 `/api/tags`로 확인하고 `qwen3:8b`가 없을 때 안내를 추가한다.

## 핵심 정리

- 실행 시 Ollama 서버가 꺼져 있으면 자동으로 시작한다.
- 서버 준비 후 `qwen3:8b`로 응답을 생성한다.
- 앱 종료 시 Ollama 서버도 함께 종료한다.
- 빌드와 실제 실행 흐름 확인을 완료했다.
