# KingSejong (훈민정음)

Unreal Engine 5 멀티플레이 미니게임 팀 프로젝트입니다.
OnlineSubsystem 기반의 세션 생성/검색/참가와 로비, 미니게임(달리기·퀴즈·배틀)을 구현했습니다.

## 기술 스택
- Unreal Engine 5 / C++
- OnlineSubsystem (테스트: LAN, 릴리즈: Steam 분기)
- Listen Server, RPC

## 코드 구조
작업은 팀원별 폴더로 분리되어 있습니다.

| 폴더 | 담당자 | 주요 담당 |
|---|---|---|
| `Source/KingSejong/*/JJH` | 조준혁 ([@AIMHYUK](https://github.com/AIMHYUK)) | 멀티플레이 세션 생성/검색/참가, 로비 UI, 달리기 미니게임 진행, 연결 실패 시 로비 복귀 예외 처리 |
| `Source/KingSejong/*/HJS` | (팀원 작성 부탁드립니다) | 배틀 게임모드 등 |
| `Source/KingSejong/*/KJH` | (팀원 작성 부탁드립니다) | |

## 주요 구현 (세션 흐름)
- `JJH_GameInstance`에서 `IOnlineSessionInterface`를 보관하고 세션 완료 델리게이트 등록
- `FOnlineSessionSettings`에 Category / Room_Name / Host_Name을 세션 광고 설정값으로 등록
- 호스트는 `ServerTravel`, 클라이언트는 `ClientTravel`로 맵 진입 경로 분리
- 호스트 이탈 또는 연결 실패 시 로비로 복귀하는 예외 처리
