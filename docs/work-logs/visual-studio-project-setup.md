# Visual Studio 프로젝트 전환 작업 기록

## 설명

사용자는 CMake를 사용하지 않고 Visual Studio를 사용하겠다고 지시했다.

이에 따라 C++ 로컬 AI 채팅 assistant 프로젝트를 Visual Studio 솔루션 기준으로 열고 빌드할 수 있도록 정리했다.

## 사용자 지시 내용

1. CMake를 사용하지 않는다.
2. Visual Studio를 사용한다.
3. 이를 위한 프로젝트 작업을 진행한다.

## assistant가 작업한 내용

1. Visual Studio 2022 솔루션 파일 `LocalAIChat.sln`을 추가했다.
2. Visual Studio C++ 프로젝트 파일 `LocalAIChat.vcxproj`를 추가했다.
3. Visual Studio 필터 파일 `LocalAIChat.vcxproj.filters`를 추가했다.
4. Debug/Release x64 구성을 추가했다.
5. C++17 표준을 사용하도록 설정했다.
6. `src` 폴더를 include 경로로 추가했다.
7. Windows 소켓 사용을 위해 `ws2_32.lib` 링크 설정을 추가했다.
8. Visual Studio 빌드 산출물을 제외하기 위한 `.gitignore`를 추가했다.
9. 기존 `CMakeLists.txt` 파일을 제거했다.
10. MSVC UTF-8 컴파일을 위해 프로젝트에 `/utf-8` 옵션을 추가했다.
11. Visual Studio 보안 경고를 피하기 위해 Windows 빌드에서 `_dupenv_s`를 사용하도록 `src/main.cpp`를 수정했다.
12. `Debug | x64` 구성을 MSBuild로 빌드해 성공 여부를 확인했다.
13. 작업 기록을 `docs/work-logs/visual-studio-project-setup.md`에 정리했다.

## assistant가 답변한 핵심 내용

Visual Studio에서 `LocalAIChat.sln`을 열고 `Debug | x64` 또는 `Release | x64` 구성으로 빌드하면 된다.

Ollama 연동을 실행하려면 실행 전에 다음 명령으로 Ollama 서버와 모델을 준비해야 한다.

```text
ollama serve
ollama pull qwen3:8b
```

## 생성하거나 수정한 파일

| 파일 | 내용 |
|---|---|
| `LocalAIChat.sln` | Visual Studio 솔루션 파일 |
| `LocalAIChat.vcxproj` | Visual Studio C++ 프로젝트 설정 |
| `LocalAIChat.vcxproj.filters` | Visual Studio 파일 필터 설정 |
| `.gitignore` | Visual Studio 빌드 산출물 제외 규칙 |
| `README.md` | Visual Studio 실행 안내 |
| `src/main.cpp` | Visual Studio 빌드 경고 방지를 위한 환경 변수 읽기 방식 수정 |
| `docs/work-logs/visual-studio-project-setup.md` | 이번 작업 기록 |
| `CMakeLists.txt` | 제거 |

## 빌드 확인 결과

다음 구성으로 빌드 확인을 완료했다.

```text
Configuration=Debug
Platform=x64
```

MSBuild 결과는 경고 0개, 오류 0개이다.

## 다음에 이어서 할 수 있는 작업

1. Release x64 구성도 빌드 확인한다.
2. 실행 시 콘솔 한글 출력이 깨지지 않도록 문자 인코딩 설정을 점검한다.
3. Visual Studio 디버깅 환경에서 `OLLAMA_MODEL` 환경 변수를 설정하는 방법을 문서화한다.

## 핵심 정리

- 프로젝트 빌드 기준을 Visual Studio 솔루션 중심으로 정리했다.
- Windows 전용 Ollama HTTP 클라이언트가 링크될 수 있도록 `ws2_32.lib`를 설정했다.
- 작업 기록은 `docs/work-logs/` 아래에 별도 문서로 남겼다.
