# 텍스트 MUD 콘솔 UTF-8 설정 작업 기록

## 설명

사용자는 MUD 게임의 한글 글자가 깨져 보인다고 알렸다.

이번 작업에서는 Windows 콘솔에서 한글 출력이 깨질 가능성을 줄이기 위해 `TextMUDGame` 실행 시작 시 콘솔 입출력 코드 페이지를 UTF-8로 설정했다.

## 핵심 정리

## 사용자가 지시한 내용

1. MUD 게임의 한글 글자가 깨져 보이는 문제를 해결한다.

## assistant가 작업한 내용

1. `TextMUDGame` 프로젝트가 이미 `/utf-8` 컴파일 옵션을 사용하고 있음을 확인했다.
2. `TextMUDGame/src/main.cpp`에 Windows 전용 콘솔 UTF-8 설정 코드를 추가했다.
3. `SetConsoleOutputCP(CP_UTF8)`로 콘솔 출력 코드 페이지를 UTF-8로 설정했다.
4. `SetConsoleCP(CP_UTF8)`로 콘솔 입력 코드 페이지를 UTF-8로 설정했다.

## assistant가 답변한 핵심 내용

1. 소스 파일과 컴파일 옵션은 UTF-8 기준이다.
2. 한글 깨짐은 Windows 콘솔 코드 페이지가 UTF-8이 아닐 때 발생할 수 있다.
3. 프로그램 시작 시 콘솔 코드 페이지를 UTF-8로 바꾸도록 수정했다.

## 생성하거나 수정한 파일

| 파일 | 작업 내용 |
|---|---|
| `TextMUDGame/src/main.cpp` | Windows 콘솔 UTF-8 설정 추가 |
| `docs/work-logs/text-mud-console-utf8.md` | 이번 작업 기록 생성 |

## 다음에 이어서 할 수 있는 작업

1. Visual Studio에서 `TextMUDGame.sln`을 다시 빌드하고 실행한다.
2. 그래도 깨져 보이면 Visual Studio 디버그 콘솔 또는 Windows Terminal의 글꼴과 코드 페이지를 확인한다.
3. 한글 명령어 입력까지 지원할 경우 입력 처리와 문자열 비교 방식도 함께 점검한다.
