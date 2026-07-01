# LocalAIChat 종료 시 Ollama 서버 종료 작업 기록

## 설명

사용자는 `LocalAIChat.exe`를 종료할 때 Ollama 서버도 함께 종료하는 기능을 추가해 달라고 지시했다.

이에 따라 앱 종료 흐름에 Windows `ollama.exe` 프로세스를 찾아 종료하는 기능을 추가했다.

## 사용자 지시 내용

1. LocalAIChat 종료 시 Ollama 서버까지 같이 끈다.

## assistant가 작업한 내용

1. `FChatApp::Run()` 종료 직후 `ShutdownOllamaServer()`를 호출하도록 수정했다.
2. Windows Tool Help API로 실행 중인 프로세스 목록을 조회했다.
3. 프로세스 이름이 `ollama.exe`인 항목을 찾아 종료하도록 구현했다.
4. 종료 성공, 종료 실패, 실행 중인 서버 없음 상태를 콘솔에 출력하도록 했다.
5. README에 프로그램 종료 시 Ollama 서버도 함께 종료된다는 내용을 추가했다.
6. `Debug | x64` 구성을 MSBuild로 빌드해 성공 여부를 확인했다.
7. 실행 후 Ollama 프로세스가 종료되는지 확인했다.

## assistant가 답변한 핵심 내용

이제 `LocalAIChat.exe`가 종료되면 실행 중인 `ollama.exe` 프로세스를 찾아 함께 종료한다.

## 생성하거나 수정한 파일

| 파일 | 내용 |
|---|---|
| `LocalAIChat/src/ChatApp.h` | Ollama 서버 종료 함수 선언 추가 |
| `LocalAIChat/src/ChatApp.cpp` | 종료 시 `ollama.exe` 프로세스 종료 기능 추가 |
| `README.md` | 종료 시 Ollama 서버도 함께 종료한다는 안내 추가 |
| `docs/work-logs/ollama-shutdown-on-exit.md` | 이번 작업 기록 |

## 확인 결과

다음 구성으로 빌드 확인을 완료했다.

```text
Configuration=Debug
Platform=x64
```

MSBuild 결과는 경고 0개, 오류 0개이다.

실행 종료 시 다음 문구가 출력되는 것을 확인했다.

```text
Ollama 서버를 종료했습니다.
```

종료 후 `ollama` 프로세스가 남아 있지 않은 것도 확인했다.

## 다음에 이어서 할 수 있는 작업

1. 앱이 직접 실행한 Ollama 서버만 종료하고, 사용자가 미리 켜둔 서버는 유지하는 옵션을 추가한다.
2. `/exit` 입력 테스트가 더 깔끔하게 동작하도록 테스트용 입력 방식을 정리한다.
3. 종료 설정을 켜고 끌 수 있는 설정 파일을 추가한다.

## 핵심 정리

- LocalAIChat 종료 시 Ollama 서버도 함께 종료되도록 수정했다.
- Windows에서는 `ollama.exe` 프로세스를 찾아 종료한다.
- 빌드와 실제 종료 동작 확인을 완료했다.
