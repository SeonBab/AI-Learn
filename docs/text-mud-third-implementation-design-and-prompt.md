# C++ MUD 게임 3차 구현 설계 문서와 코드 요청 프롬프트

## 설명

이 문서는 C++ 텍스트 MUD 게임의 3차 구현 설계와 코드 요청 프롬프트를 정리한다.

3차 구현의 목표는 `save`와 `load` 명령어를 추가해 현재 방, 인벤토리, 성문 잠금 해제 상태, 성 안뜰 도착 여부를 단일 `save.txt` 파일에 저장하고 다시 불러오는 것이다.

## 핵심 정리

1. `save` 명령어로 현재 게임 상태를 `save.txt`에 저장한다.
2. `load` 명령어로 `save.txt`에서 게임 상태를 복원한다.
3. 저장 대상은 `currentRoomId`, `inventory`, `castleGateUnlocked`, `reachedCourtyard`다.
4. `SaveManager` 클래스를 추가한다.
5. `load` 후 인벤토리에 있는 아이템은 방에서 제거해 중복을 방지한다.
6. JSON, 여러 저장 슬롯, 자동 저장, AI NPC 기억 저장은 이번 단계에서 제외한다.

## 저장 파일 형식

```text
currentRoomId=castle_courtyard
inventory=old_key
castleGateUnlocked=true
reachedCourtyard=true
```

인벤토리가 비어 있으면 다음처럼 저장한다.

```text
inventory=
```

## 추가 명령어

```text
save
load
```

`save` 성공 메시지:

```text
게임을 저장했다.
```

`load` 성공 메시지:

```text
게임을 불러왔다.
```

실패 메시지:

```text
게임 저장에 실패했다.
게임 불러오기에 실패했다.
```

## SaveManager 역할

`SaveManager`는 파일 저장과 불러오기를 담당한다.

```text
1. Player 상태를 파일에 저장
2. GameState 상태를 파일에 저장
3. save.txt 파일 읽기
4. 읽은 값을 Player와 GameState에 반영
5. inventory 문자열을 vector<Item>으로 변환
6. vector<Item>을 inventory 문자열로 변환
7. true / false 문자열을 bool로 변환
8. 파일 입출력 실패 시 false 반환
```

## GameEngine 수정

`GameEngine`은 다음을 가진다.

```text
SaveManager saveManager
saveFilePath = "save.txt"
```

추가 함수:

```text
handleSave()
handleLoad()
```

`handleLoad()`는 저장된 현재 방 ID가 실제 월드에 존재하는지 확인하고, 불러온 인벤토리 아이템을 모든 방에서 제거한다.

## World 수정

`load` 후 아이템 중복을 막기 위해 다음 기능을 추가한다.

```text
removeItemFromAllRooms(itemId)
removeInventoryItemsFromRooms(inventory)
```

## 정상 테스트 시나리오

첫 번째 실행:

```text
look
go east
take old_key
go west
go south
use old_key
go south
save
quit
```

두 번째 실행:

```text
load
inventory
look
go north
go south
quit
```

## 실패 테스트 시나리오

```text
load
```

저장 파일이 없거나 잘못된 경우에도 프로그램이 종료되지 않아야 한다.

아이템 중복 확인:

```text
go east
take old_key
save
quit
```

다시 실행:

```text
load
go west
go east
items
```

잡화점에 `old_key`가 다시 나타나지 않아야 한다.

## 코드 요청 프롬프트

```text
너는 C++ 학습을 도와주는 프로그래밍 assistant다.

콘솔 텍스트 MUD 게임의 3차 버전을 구현해줘.

목표:
- save 명령어 추가
- load 명령어 추가
- SaveManager 클래스 추가
- currentRoomId 저장
- inventory 저장
- castleGateUnlocked 저장
- reachedCourtyard 저장
- load 성공 후 현재 방 설명 출력
- load 후 인벤토리 아이템과 방 아이템 중복 제거

저장 파일:
save.txt

저장 형식:
currentRoomId=castle_courtyard
inventory=old_key
castleGateUnlocked=true
reachedCourtyard=true

완료 기준:
1. save.txt 파일이 생성된다.
2. load로 현재 방, 인벤토리, GameState가 복원된다.
3. 저장 파일이 없어도 프로그램이 종료되지 않는다.
4. 잘못된 저장 파일이어도 안전하게 실패한다.
5. 불러온 뒤 이미 가진 아이템이 방에 다시 나타나지 않는다.
```

## 다음 확장

4차 구현에서는 로컬 LLM 기반 AI NPC 응답 기능을 추가한다.

예상 명령어는 다음과 같다.

```text
ask guard 성 안에는 뭐가 있나요?
ask merchant 열쇠는 어디에 쓰나요?
```
