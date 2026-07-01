# 프로젝트 컨벤션: Unreal Engine 코딩 스타일 기반

## 설명

이 문서는 C++ 학습용 로컬 AI 채팅 프로그램에서 사용할 프로젝트 컨벤션을 정리한다.

기본 방향은 Unreal Engine의 C++ 코딩 스타일을 따른다. 단, 이 프로젝트가 Unreal Engine 프로젝트가 아닐 수 있으므로 Unreal 전용 매크로, 리플렉션 시스템, 빌드 도구에 강하게 묶이는 규칙은 그대로 강제하지 않고 일반 C++ 학습 프로젝트에 맞게 적용한다.

참고 기준:

- Unreal Engine C++ 코딩 표준
- Unreal Engine C++ API 및 엔진 코드에서 사용되는 명명 규칙
- 읽기 쉬운 C++ 학습 코드 작성 원칙

## 예제 코드

```cpp
class FChatMessage
{
public:
    FChatMessage(const FString& InSenderName, const FString& InContent)
        : SenderName(InSenderName)
        , Content(InContent)
        , bIsRead(false)
    {
    }

    void MarkAsRead()
    {
        bIsRead = true;
    }

    bool IsRead() const
    {
        return bIsRead;
    }

private:
    FString SenderName;
    FString Content;
    bool bIsRead;
};
```

## 핵심 정리

### 1. 파일 이름 규칙

파일 이름은 타입 이름과 최대한 일치시킨다.

```text
ChatMessage.h
ChatMessage.cpp
ChatSession.h
ChatSession.cpp
```

하나의 주요 클래스가 있다면 헤더와 구현 파일 이름은 그 클래스 이름을 기준으로 한다.

### 2. 타입 이름 규칙

Unreal Engine 스타일에서는 타입의 성격을 접두사로 표현한다.

| 종류 | 접두사 | 예시 | 설명 |
|---|---:|---|---|
| 일반 클래스 또는 구조체 성격의 타입 | `F` | `FChatMessage` | 값 타입, 데이터 구조, 일반 C++ 클래스 |
| Unreal Object 계열 클래스 | `U` | `UChatManager` | Unreal의 `UObject` 기반 클래스에 사용 |
| Actor 계열 클래스 | `A` | `AChatBotActor` | Unreal의 `AActor` 기반 클래스에 사용 |
| Interface | `I` | `IMessageProvider` | 인터페이스 타입 |
| Template 타입 | `T` | `TArray`, `TSharedPtr` | 템플릿 타입 |
| Enum | `E` | `EMessageRole` | 열거형 |
| Boolean 변수 | `b` | `bIsRead` | 참 또는 거짓 상태 |

이 프로젝트에서는 Unreal Engine을 직접 사용하지 않는 일반 C++ 코드라면 다음 규칙을 우선한다.

1. 학습용 데이터 타입과 일반 클래스는 `F` 접두사를 사용한다.
2. 인터페이스는 `I` 접두사를 사용한다.
3. 열거형은 `E` 접두사를 사용한다.
4. Boolean 변수는 `b` 접두사를 사용한다.
5. Unreal 전용 타입인 `U`, `A` 접두사는 Unreal 기반 코드에서만 사용한다.

### 3. 변수와 함수 이름 규칙

함수, 클래스, 구조체, 열거형 이름은 PascalCase를 사용한다.

```cpp
void SendMessage();
void ClearHistory();
int32 GetMessageCount() const;
```

지역 변수와 멤버 변수도 Unreal 스타일에 맞춰 PascalCase를 사용한다.

```cpp
FString UserName;
int32 MessageCount;
bool bIsConnected;
```

축약어는 과하게 사용하지 않는다.

```cpp
// 권장
int32 MessageCount;

// 비권장
int32 MsgCnt;
```

### 4. Boolean 이름 규칙

Boolean 변수는 `b` 접두사를 사용하고, 의미가 자연스럽게 읽히도록 작성한다.

```cpp
bool bIsActive;
bool bHasError;
bool bCanSendMessage;
```

함수로 상태를 확인할 때는 `Is`, `Has`, `Can`, `Should`를 사용한다.

```cpp
bool IsConnected() const;
bool HasMessages() const;
bool CanSendMessage() const;
bool ShouldRetry() const;
```

### 5. 상수와 Enum 규칙

상수는 의미가 분명한 PascalCase 이름을 사용한다.

```cpp
constexpr int32 MaxMessageLength = 1000;
constexpr float RetryDelaySeconds = 1.5f;
```

Enum은 `E` 접두사를 사용하고, 값 이름은 PascalCase로 작성한다.

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

각 접근 제어 영역 안에서는 다음 순서를 권장한다.

1. 생성자와 소멸자
2. 주요 public 함수
3. 보조 함수
4. 멤버 변수

```cpp
class FChatSession
{
public:
    FChatSession();

    void AddMessage(const FChatMessage& Message);
    int32 GetMessageCount() const;

private:
    TArray<FChatMessage> Messages;
};
```

### 7. 포인터와 참조 규칙

입력값을 수정하지 않는 큰 객체는 `const` 참조로 전달한다.

```cpp
void AddMessage(const FChatMessage& Message);
```

소유권을 표현해야 하는 경우에는 스마트 포인터를 우선 고려한다.

```cpp
TSharedPtr<FChatSession> ActiveSession;
TUniquePtr<FChatTokenizer> Tokenizer;
```

일반 C++ 표준 라이브러리를 사용하는 코드라면 아래처럼 대응한다.

```cpp
std::shared_ptr<FChatSession> ActiveSession;
std::unique_ptr<FChatTokenizer> Tokenizer;
```

### 8. `const` 사용 규칙

객체 상태를 변경하지 않는 멤버 함수에는 `const`를 붙인다.

```cpp
int32 GetMessageCount() const;
bool IsEmpty() const;
```

입력 인자를 수정하지 않을 때는 `const`를 적극적으로 사용한다.

```cpp
void PrintMessage(const FChatMessage& Message);
```

### 9. 중괄호와 들여쓰기

중괄호는 Unreal Engine 스타일처럼 새 줄에 작성한다.

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

들여쓰기는 탭 또는 스페이스 중 프로젝트 설정을 따른다. 이 프로젝트에서는 문서와 예제 코드의 일관성을 위해 스페이스 4칸을 기본으로 한다.

### 10. 주석 규칙

주석은 코드가 무엇을 하는지 반복하지 말고, 왜 그렇게 작성했는지를 설명한다.

```cpp
// 메시지 순서를 보존해야 하므로 삭제 대신 읽음 상태만 갱신한다.
Message.MarkAsRead();
```

공개 API에는 짧은 설명을 붙이는 것을 권장한다.

```cpp
// 현재 세션에 저장된 메시지 개수를 반환한다.
int32 GetMessageCount() const;
```

### 11. 헤더 파일 규칙

헤더 파일에는 필요한 선언만 둔다.

가능하면 전방 선언을 사용하고, 구현 파일에서 실제 헤더를 포함한다.

```cpp
class FChatMessage;

class FChatSession
{
public:
    void AddMessage(const FChatMessage& Message);
};
```

헤더 포함 순서는 다음을 권장한다.

1. 자기 자신의 헤더
2. 프로젝트 내부 헤더
3. 외부 라이브러리 헤더
4. C++ 표준 라이브러리 헤더

### 12. 함수 작성 규칙

함수는 한 가지 책임만 갖도록 작성한다.

```cpp
void FChatSession::AddMessage(const FChatMessage& Message)
{
    Messages.Add(Message);
}
```

함수가 길어지면 의미 있는 작은 함수로 분리한다.

### 13. 학습 코드 작성 규칙

이 프로젝트는 C++ 학습을 목적으로 하므로 다음 원칙을 추가로 따른다.

1. 예제 코드는 동작 의도가 분명해야 한다.
2. 어려운 문법을 사용할 때는 짧은 주석을 추가한다.
3. Unreal 스타일과 표준 C++ 스타일이 충돌할 때는 문서에서 차이를 설명한다.
4. 학습 초반에는 과도한 템플릿, 매크로, 메타프로그래밍을 피한다.
5. 코드보다 개념 설명이 중요할 때는 예제를 작게 유지한다.

### 14. 권장하지 않는 스타일

의미 없는 축약어를 피한다.

```cpp
// 비권장
int32 Cnt;
FString Msg;
```

Boolean 변수에서 의미가 불분명한 이름을 피한다.

```cpp
// 비권장
bool bCheck;
bool bFlag;
```

함수 이름에 동작이 드러나지 않는 표현을 피한다.

```cpp
// 비권장
void Process();
void Handle();
```

대신 구체적인 이름을 사용한다.

```cpp
void ProcessUserMessage();
void HandleConnectionError();
```

### 15. 문서 작성 규칙

프로젝트 문서는 기본적으로 한글로 작성한다.

단, 다음 항목은 영어를 그대로 사용할 수 있다.

1. C++ 키워드
2. API 이름
3. 클래스, 함수, 변수 이름
4. 컴파일러 에러 메시지
5. Unreal Engine 또는 외부 라이브러리의 공식 용어

문서에 C++ 코드가 필요한 경우 반드시 `cpp` 코드 블록을 사용한다.