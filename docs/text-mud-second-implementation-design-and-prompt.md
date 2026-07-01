# C++ MUD 게임 2차 구현 설계 문서와 코드 요청 프롬프트

## 설명

이 문서는 C++ 텍스트 MUD 게임의 2차 구현 설계와 코드 요청 프롬프트를 정리한다.

2차 구현의 목표는 `use old_key` 명령어, 성문 잠금 해제 상태, 성 안뜰 이동, 첫 목표 달성 메시지를 추가하는 것이다. 핵심은 단순히 명령어 하나를 늘리는 것이 아니라 `명령어 입력 -> 게임 상태 변경 -> 결과 출력` 흐름에서 게임 상태가 실제로 바뀌고, 그 상태에 따라 이동 가능 여부가 달라지도록 만드는 것이다.

## 핵심 정리

1. `use old_key` 명령어를 추가한다.
2. `GameState` 클래스로 `castleGateUnlocked`, `reachedCourtyard` 상태를 관리한다.
3. `castle_gate`에서 `south` 이동은 성문이 열린 뒤에만 허용한다.
4. `castle_courtyard` 방을 추가한다.
5. 성 안뜰에 처음 도착하면 `첫 번째 목표를 달성했다!` 메시지를 한 번만 출력한다.
6. `save/load`, AI NPC, 전투, 퀘스트 시스템은 이번 단계에서 제외한다.

## 2차 구현 기능 목록

```text
1. use 명령어 추가
2. old_key 사용 처리
3. old_key는 성문 앞에서만 사용 가능
4. 성문 잠금 해제 상태 추가
5. castle_courtyard 방 추가
6. 성문이 잠겨 있으면 성 안뜰 이동 차단
7. 성문이 열리면 성 안뜰 이동 허용
8. 성 안뜰 첫 도착 시 목표 달성 메시지 출력
9. GameState 클래스 추가
10. 정상 흐름과 실패 흐름 테스트 시나리오 작성
```

## 제외할 기능

```text
1. save / load 저장 기능
2. 로컬 LLM 기반 AI NPC ask 기능
3. 퀘스트 시스템
4. 여러 개의 잠긴 문
5. 여러 아이템 사용 효과
6. 아이템 사용 후 제거 여부 처리
7. 전투
8. 체력
9. 레벨
10. 경험치
11. 상점 구매
12. 네트워크 기능
```

## 추가 명령어

```text
use old_key
```

결과 메시지는 다음 기준을 사용한다.

| 상황 | 메시지 |
|---|---|
| 열쇠가 없음 | `그런 아이템을 가지고 있지 않다.` |
| 성문 앞이 아님 | `여기서는 낡은 열쇠를 사용할 수 없다.` |
| 이미 열림 | `성문은 이미 열려 있다.` |
| 정상 사용 | `낡은 열쇠를 사용해 성문을 열었다.` |

## GameState

`GameState`는 플레이어 위치나 인벤토리가 아니라 월드 진행 상태를 관리한다.

```cpp
class GameState {
private:
    bool castleGateUnlocked;
    bool reachedCourtyard;

public:
    GameState();

    bool isCastleGateUnlocked() const;
    void unlockCastleGate();

    bool hasReachedCourtyard() const;
    void setReachedCourtyard(bool value);
};
```

## 추가 방

```text
ID: castle_courtyard
이름: 성 안뜰
설명: 오래된 성 안뜰이다. 낡은 돌바닥과 무너진 분수가 보인다.
이동 가능:
- north -> castle_gate
아이템: 없음
NPC: 없음
```

## 기존 방 수정

`castle_gate`에 다음 이동 경로를 추가한다.

```text
south -> castle_courtyard
```

단, `castleGateUnlocked == false`이면 이동할 수 없어야 한다.

## 정상 테스트 시나리오

```text
look
go east
take old_key
go west
go south
use old_key
go south
look
quit
```

## 실패 테스트 시나리오

```text
use old_key
```

```text
go east
take old_key
use old_key
```

```text
go south
go south
```

```text
use wrong_item
```

```text
use old_key
use old_key
```

## 코드 요청 프롬프트

```text
너는 C++ 학습을 도와주는 프로그래밍 assistant다.

콘솔 텍스트 MUD 게임의 2차 버전을 구현해줘.

목표:
- use old_key 명령어 추가
- GameState 클래스 추가
- castleGateUnlocked 상태 추가
- reachedCourtyard 상태 추가
- castle_courtyard 방 추가
- castle_gate에서 south 이동 시 잠금 상태 검사
- 성 안뜰 첫 도착 시 "첫 번째 목표를 달성했다!"를 한 번만 출력

제외:
- save/load
- 로컬 LLM ask
- 전투
- 퀘스트 시스템
- 네트워크

완료 기준:
look
go east
take old_key
go west
go south
use old_key
go south
look
quit
```

## 다음 확장

3차 구현에서는 저장과 불러오기를 추가한다.

저장 대상 후보는 다음과 같다.

```text
currentRoomId
inventory
castleGateUnlocked
reachedCourtyard
```
