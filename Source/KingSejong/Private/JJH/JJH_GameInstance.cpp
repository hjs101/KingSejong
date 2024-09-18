// Fill out your copyright notice in the Description page of Project Settings.


#include "JJH/JJH_GameInstance.h"
#include "OnlineSubSystem.h"
#include "../../../../Plugins/Online/OnlineSubsystem/Source/Public/Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "../../../../Plugins/Online/OnlineBase/Source/Public/Online/OnlineSessionNames.h"
#include "Online/CoreOnline.h"
#include "JJH/LobbyWidget.h"

const static FName SESSION_NAME = TEXT("My Session Game");
const static FName SESSION_CATEGORY = TEXT("RUN");

UJJH_GameInstance::UJJH_GameInstance()
{
	
}

void UJJH_GameInstance::Init()
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem)
	{
		SessionInterface = Subsystem->GetSessionInterface();
	}
		
	SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UJJH_GameInstance::OnMyCreateSessionComplete);
	SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UJJH_GameInstance::OnMyFindSessionComplete);

}

void UJJH_GameInstance::LoadMenu()
{
	ULobbyWidget* Lobby = CreateWidget<ULobbyWidget>(GetWorld(), MenuClass);
	
	if(Lobby == nullptr) return;

	Lobby->AddToViewport();

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if ( !ensure(PlayerController != nullptr) ) return;

	FInputModeUIOnly InputModeData;
	InputModeData.SetWidgetToFocus(Lobby->TakeWidget());
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	PlayerController->SetInputMode(InputModeData);

	PlayerController->bShowMouseCursor = true;

	Lobby->SetMenuInterface(this);
}

//void UJJH_GameInstance::Host()
//{
//	if ( SessionInterface.IsValid() )
//	{
//		auto ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);
//		if ( ExistingSession != nullptr )
//		{
//			SessionInterface->DestroySession(SESSION_NAME);
//		}
//		else
//		{
//			//CreateSession();
//		}
//	}
//}

void UJJH_GameInstance::JoinToSession(int32 Index)
{
	auto result = SessionSearch->SearchResults[Index];
	SessionInterface->JoinSession(0, FName(MySessionName), result);
}

void UJJH_GameInstance::CreateSession(const FString& RoomName, int32 PlayerCount)
{
	if ( SessionInterface )
	{
		FOnlineSessionSettings SessionSettings;

		//로비 사용
		SessionSettings.bUseLobbiesIfAvailable = true;

		//1.데디케이트 서버인가?
		SessionSettings.bIsDedicated = false;

		//2.Lan으로 매치하는가? , 서브시스템이 널이라면 랜매치겠죠? 아니면 스팀이라 뜰꺼니까
		SessionSettings.bIsLANMatch = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL";

		//3. 매칭이 공개인지 & 비공개인지
		SessionSettings.bShouldAdvertise = true;//공개

		//4. 유저의 상태 정보(온라인/ 자리비움 등등) 사용여부
		SessionSettings.bUsesPresence = true;

		//5. 중간에 난입가능한가?
		SessionSettings.bAllowJoinInProgress = true;
		SessionSettings.bAllowJoinViaPresence = true;

		//6. 몇명이 접속할래?
		SessionSettings.NumPublicConnections = PlayerCount;

		// 세션 설정 시 카테고리 지정 -> 추후에 
		SessionSettings.Set<FString>(FName("Category"), SESSION_CATEGORY.ToString(), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
		SessionSettings.Set<FString>(FName("Room_Name"), RoomName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
		SessionSettings.Set<FString>(FName("Host_Name"), SESSION_NAME.ToString(), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

		FUniqueNetIdPtr netID = GetWorld()->GetFirstLocalPlayerFromController()->GetUniqueNetIdForPlatformUser().GetUniqueNetId();
		SessionInterface->CreateSession(*netID, SESSION_NAME, SessionSettings);
		UE_LOG(LogTemp, Warning, TEXT("123"));
	}
}

void UJJH_GameInstance::OnMyCreateSessionComplete(FName SessionName, bool Success)
{
	if ( !Success )
	{
		return;
	}

	GEngine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Hosting"));

	GetWorld()->ServerTravel("/Game/JJH/ProtoRunningMap?listen");
}

void UJJH_GameInstance::OnMyDestroySessionComplete(FName SessionName, bool Success)
{
	////파괴에 성공하면 다시 만들기
	//if ( Success )
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Session Destroyed"));
	//	CreateSession(SessionName, PlayerCount);
	//}
}

void UJJH_GameInstance::OnMyFindSessionComplete(bool Success)
{
	if ( Success )
	{
		//방 만든 결과들 배열에 담아서
		TArray<FOnlineSessionSearchResult> results = SessionSearch->SearchResults;
		for ( int32 i = 0; i < results.Num(); i++ )
		{
			if ( false == results[ i ].IsValid() )
			{
				continue;
			}
			//방 정보
			FRoomInfo roomInfo;
			//카테고리
			FString categoryString;
			if ( results[ i ].Session.SessionSettings.Get<FString>(FName("Category"), categoryString) )
			{
				if ( categoryString == "Run" )
					roomInfo.RoomCategory = ERoomCategory::Run;
				else if ( categoryString == "Battle" )
					roomInfo.RoomCategory = ERoomCategory::Battle;
				else if ( categoryString == "Talk" )
					roomInfo.RoomCategory = ERoomCategory::Talk;
			}
			roomInfo.index = i;

			FString roomNameString;
			results[ i ].Session.SessionSettings.Get<FString>(FName("Room_Name"), roomNameString);
			roomInfo.roomName = roomNameString;

			FString hostNameString;
			results[ i ].Session.SessionSettings.Get<FString>(FName("Host_Name"), hostNameString);
			roomInfo.hostName = hostNameString;
			//최대 플레이어 수
			roomInfo.MaxPlayerCount = results[ i ].Session.SessionSettings.NumPublicConnections;
			//입장 가능한 플레이어 최대 - 지금
			roomInfo.CurrentPlayerCount = roomInfo.MaxPlayerCount - results[ i ].Session.NumOpenPublicConnections;
			//핑 정보
			roomInfo.pingMS = results[ i ].PingInMs;


			//델리게이트 룸 생성
			if(OnSearchSignatureCompleteDelegate.IsBound()) OnSearchSignatureCompleteDelegate.Broadcast(roomInfo);
		}
	}
}

//void UJJH_GameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
//{
//
//}


