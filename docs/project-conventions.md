# 프로젝트 컨벤션: Unreal Engine 스타일 기반 C++ 규칙

## 설명

이 문서는 C++ 학습용 로컬 AI 채팅 프로그램에서 사용할 프로젝트 코딩 컨벤션을 정리한다.

기본 방향은 Unreal Engine의 C++ 코딩 스타일을 참고한다. 다만 이 프로젝트는 Unreal Engine 프로젝트가 아니므로 Unreal 전용 매크로, 리플렉션 시스템, 빌드 규칙을 강제하지 않는다. 읽기 쉬운 학습용 C++ 코드 작성에 필요한 부분만 적용한다.

## 핵심 정리

### 1. 파일 이름 규칙

파일 이름은 주요 클래스 이름과 최대한 일치시킨다.

```text
ChatMessage.h
ChatMessage.cpp
ChatApp.h
ChatApp.cpp
AIEngine.h
AIEngine.cpp
```

### 2. 타입 이름 규칙

| 종류 | 접두사 | 예시 | 설명 |
|---|---:|---|---|
| 일반 클래스 또는 값 타입 | `F` | `FChatMessage` | 값처럼 다루는 데이터 구조 또는 일반 클래스 |
| 인터페이스 | `I` | `IMessageProvider` | 인터페이스 역할 |
| enum class | `E` | `EMessageRole` | 열거형 |
| boolean 변수 | `b` | `bIsConnected` | 참 또는 거짓 상태 |

Unreal 전용 타입인 `U`, `A` 접두사는 Unreal 기반 코드에서만 사용한다.

### 3. 함수와 변수 이름 규칙

함수, 클래스, 구조체, enum 이름은 PascalCase를 사용한다.

```cpp
void SendMessage();
void ClearHistory();
int GetMessageCount() const;
```

축약어는 과하게 사용하지 않는다.

```cpp
// 권장
int MessageCount;

// 비권장
int MsgCnt;
```

### 4. boolean 이름 규칙

boolean 변수는 `b` 접두사를 사용하고 의미가 자연스럽게 읽히도록 작성한다.

```cpp
bool bIsActive;
bool bHasError;
bool bCanSendMessage;
```

상태 확인 함수는 `Is`, `Has`, `Can`, `Should`를 사용한다.

```cpp
bool IsConnected() const;
bool HasMessages() const;
bool CanSendMessage() const;
bool ShouldRetry() const;
```

### 5. 상수와 enum 규칙

상수는 의미가 분명한 PascalCase 이름을 사용한다.

```cpp
constexpr int MaxMessageLength = 1000;
```

enum은 `E` 접두사를 사용하고 값 이름은 PascalCase로 작성한다.

```cpp
enum class EMessageRole
{
    User,
    Assistant,
    System
};
```

### 6. 클래스 작성 순서

클래스 내부는 읽기 쉬운 순서로 작성한다.

1. `public`
2. `protected`
3. `private`

각 접근 제어 영역 안에서는 생성자, 주요 함수, 보조 함수, 멤버 변수 순서를 권장한다.

```cpp
class FChatSession
{
public:
    FChatSession();

    void AddMessage(const FChatMessage& Message);
    int GetMessageCount() const;

private:
    std::vector<FChatMessage> Messages;
};
```

### 7. 참조와 스마트 포인터 규칙

입력값을 수정하지 않는 큰 객체는 `const` 참조로 전달한다.

```cpp
void AddMessage(const FChatMessage& Message);
```

소유권을 표현해야 하는 경우에는 스마트 포인터를 우선 고려한다.

```cpp
std::unique_ptr<FAIEngine> AIEngine;
std::shared_ptr<FChatSession> ActiveSession;
```

### 8. `const` 사용 규칙

객체 상태를 변경하지 않는 멤버 함수에는 `const`를 붙인다.

```cpp
int GetMessageCount() const;
bool IsEmpty() const;
```

### 9. 중괄호와 들여쓰기

중괄호는 Unreal Engine 스타일처럼 다음 줄에 작성한다.

```cpp
if (bIsConnected)
{
    SendMessage();
}
else
{
    Reconnect();
}
```

들여쓰기는 스페이스 4칸을 기본으로 한다.

### 10. 주석 규칙

주석은 코드가 무엇을 하는지 반복하지 말고, 왜 그렇게 작성했는지를 설명한다.

```cpp
// 메시지 순서를 보존해야 하므로 삭제 대신 읽음 상태만 갱신한다.
Message.MarkAsRead();
```

### 11. 문서 작성 규칙

프로젝트 문서는 기본적으로 한글로 작성한다. C++ 키워드, API 이름, 클래스 이름, 함수 이름, 에러 메시지, 공식 용어는 영어를 그대로 사용할 수 있다.

문서에 C++ 코드가 필요하면 반드시 `cpp` 코드 블록을 사용한다.
