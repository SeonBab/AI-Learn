# UTF-8 인코딩 설정 작업 기록

## 설명

사용자는 빌드 후 실행했을 때 인코딩이 깨진다고 알려주었고, `.editorconfig`를 만들어 UTF-8로 설정해 달라고 지시했다.

이에 따라 저장소의 텍스트 파일 저장 규칙을 UTF-8로 고정하고, Windows 콘솔 실행 시 입출력 코드 페이지도 UTF-8로 설정했다.

## 사용자 지시 내용

1. 빌드 실행 시 인코딩이 깨지는 문제를 해결한다.
2. `.editorconfig` 파일을 만든다.
3. UTF-8을 기본 인코딩으로 설정한다.
4. 기존 파일도 인코딩 설정을 확인하고 적용한다.

## assistant가 작업한 내용

1. 루트에 `.editorconfig` 파일을 추가했다.
2. 전체 파일 기본 문자셋을 `utf-8`로 설정했다.
3. C++ 파일은 스페이스 4칸 들여쓰기를 사용하도록 설정했다.
4. Visual Studio 프로젝트 파일은 스페이스 2칸 들여쓰기를 사용하도록 설정했다.
5. 기존 문서, 소스, 솔루션, 프로젝트 파일이 유효한 UTF-8인지 검사했다.
6. Windows 실행 콘솔에서 `SetConsoleOutputCP(CP_UTF8)`과 `SetConsoleCP(CP_UTF8)`을 호출하도록 `main.cpp`를 수정했다.
7. `Debug | x64` 구성을 MSBuild로 빌드해 성공 여부를 확인했다.
8. 실행 출력에서 한글 안내문이 정상 표시되는지 확인했다.

## assistant가 답변한 핵심 내용

파일 저장 인코딩은 `.editorconfig`로 UTF-8 기준을 잡았고, 실행 중 콘솔 출력 인코딩은 Windows API로 UTF-8 코드 페이지를 설정했다.

## 생성하거나 수정한 파일

| 파일 | 내용 |
|---|---|
| `.editorconfig` | 저장소 기본 UTF-8 인코딩 규칙 추가 |
| `LocalAIChat/src/main.cpp` | Windows 콘솔 입출력 코드 페이지를 UTF-8로 설정 |
| `docs/work-logs/utf8-editorconfig-console-encoding.md` | 이번 작업 기록 |

## 확인 결과

다음 구성으로 빌드 확인을 완료했다.

```text
Configuration=Debug
Platform=x64
```

MSBuild 결과는 경고 0개, 오류 0개이다.

실행 출력에서 다음 한글 문장이 정상 표시되는 것을 확인했다.

```text
명령어는 /help 를 입력해서 확인할 수 있습니다.
Local AI Chat을 종료합니다.
```

## 다음에 이어서 할 수 있는 작업

1. Visual Studio에서 직접 실행했을 때도 한글이 정상 표시되는지 확인한다.
2. 사용자의 콘솔 글꼴이 한글을 지원하는지 확인한다.
3. AI 응답 한글이 깨질 경우 Ollama 응답 디코딩 경로를 추가 점검한다.

## 핵심 정리

- 저장소 기본 인코딩을 UTF-8로 고정했다.
- Windows 콘솔 입출력 코드 페이지도 UTF-8로 맞췄다.
- 빌드와 기본 실행 출력 확인을 완료했다.
