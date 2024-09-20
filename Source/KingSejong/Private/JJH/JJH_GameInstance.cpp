// Fill out your copyright notice in the Description page of Project Settings.


#include "JJH/JJH_GameInstance.h"
#include "OnlineSubSystem.h"
#include "../../../../Plugins/Online/OnlineSubsystem/Source/Public/Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "../../../../Plugins/Online/OnlineBase/Source/Public/Online/OnlineSessionNames.h"
#include "Online/CoreOnline.h"
#include "JJH/LobbyWidget.h"
#include "JJH/SelectCharacterInterface.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerState.h"

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
	SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UJJH_GameInstance::OnMyJoinSessionComplete);
	SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UJJH_GameInstance::OnMyDestroySessionComplete);

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
	UE_LOG(LogTemp, Warning, TEXT("hhhD"));
	auto result = SessionSearch->SearchResults[Index];
	SessionInterface->JoinSession(0, SESSION_NAME, result);
}

void UJJH_GameInstance::CreateSession(const FString& RoomName, int32 PlayerCount, const FString& Category)
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
		SessionSettings.Set<FString>(FName("Category"), Category, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
		SessionSettings.Set<FString>(FName("Room_Name"), RoomName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
		SessionSettings.Set<FString>(FName("Host_Name"), SESSION_NAME.ToString(), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

		// SessionSettings에서 값을 가져와 로그로 출력

		if ( SessionSettings.Get<FString>(FName("Category"), CategoryValue) )
		{
			UE_LOG(LogTemp, Warning, TEXT("Category: %s"), *CategoryValue);
		}

		if ( SessionSettings.Get<FString>(FName("Room_Name"), RoomNameValue) )
		{
			UE_LOG(LogTemp, Warning, TEXT("Room Name: %s"), *RoomNameValue);
		}

		if ( SessionSettings.Get<FString>(FName("Host_Name"), HostNameValue) )
		{
			UE_LOG(LogTemp, Warning, TEXT("Host Name: %s"), *HostNameValue);
		}

		FUniqueNetIdPtr netID = GetWorld()->GetFirstLocalPlayerFromController()->GetUniqueNetIdForPlatformUser().GetUniqueNetId();
		SessionInterface->CreateSession(*netID, FName(HostNameValue), SessionSettings);
	}
}
void UJJH_GameInstance::DestroySession()
{
	SessionInterface->DestroySession(SESSION_NAME);
}
void UJJH_GameInstance::OnMyCreateSessionComplete(FName SessionName, bool Success)
{
	if ( !Success )
	{
		return;
	}
	if ( CategoryValue == TEXT("Run") )
	{
		GetWorld()->ServerTravel("/Game/JJH/ProtoRunningMap?listen");
	}
	else if ( CategoryValue == TEXT("Battle") )
	{
		GetWorld()->ServerTravel("/Game/HJS/Maps/BattleTestMap?listen");
	}
	else if ( CategoryValue == TEXT("Talk") )
	{
		GetWorld()->ServerTravel("/Game/KJH/Maps/KJH_CommunityMap?listen");
	}
	GEngine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Hosting"));

}

void UJJH_GameInstance::OnMyDestroySessionComplete(FName SessionName, bool Success)
{
	//파괴에 성공하면 다시 만들기
	if ( Success )
	{
		UE_LOG(LogTemp, Warning, TEXT("Session Destroyed"));
	}
}

void UJJH_GameInstance::FindOtherSessions()
{

	UE_LOG(LogTemp, Warning, TEXT("HName"));

	SessionSearch = MakeShareable(new FOnlineSessionSearch);

	//무슨조건으로 찾을래
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
	//랜인가?
	//SessionSearch->bIsLanQuery = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL";
	//몇 개 찾을래
	SessionSearch->MaxSearchResults = 40;
	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
}



void UJJH_GameInstance::OnMyFindSessionComplete(bool Success)
{
	if ( Success && SessionSearch->SearchResults.Num() > 0)
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
			//카테고리 NN인 것만 나눠서 Find창에 띄우기
			//
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
			//방이름
			FString roomNameString;
			results[ i ].Session.SessionSettings.Get<FString>(FName("Room_Name"), roomNameString);
			roomInfo.roomName = roomNameString;
			//호스트 이름
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
	else
	{
		// 검색 결과가 없을 때 처리
		FRoomInfo EmptyRoomInfo;
		EmptyRoomInfo.roomName = TEXT("No Rooms Found");
		OnSearchSignatureCompleteDelegate.Broadcast(EmptyRoomInfo);
	}
}

void UJJH_GameInstance::OnMyJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if ( Result == EOnJoinSessionCompleteResult::Success )
	{
		//서버가 있는 레벨로 여행을 떠나고 싶다
		APlayerController* pc = GetWorld()->GetFirstPlayerController();

		FString url;
		SessionInterface->GetResolvedConnectString(SessionName, url);
		if ( false == url.IsEmpty() )
		{
			pc->ClientTravel(url, ETravelType::TRAVEL_Absolute);
		}

	}
}

void UJJH_GameInstance::SetSelectedCharacterMesh(USkeletalMesh* Mesh)
{
	SelectedCharacterMesh = Mesh;
}


//============================

//void UJJH_GameInstance::ClientUpdatePlayerMesh_Implementation(APlayerController* PC, USkeletalMesh* NewMesh)
//{
//	UE_LOG(LogTemp, Error, TEXT("qwer6"));
//	//메시 업데이트
//	ISelectCharacterInterface* UpdateablePawn = Cast<ISelectCharacterInterface>(PC->GetPawn());
//	if ( UpdateablePawn )
//	{
//		UpdateablePawn->UpdateMesh(NewMesh);
//		UE_LOG(LogTemp, Error, TEXT("qwer2"));
//	}
//}
//
//void UJJH_GameInstance::MulticastSyncAllPlayerMeshes_Implementation()
//{
//	for ( const auto& Pair : PlayerMeshes )
//	{
//		ClientUpdatePlayerMesh(Pair.Key, Pair.Value);
//		UE_LOG(LogTemp, Error, TEXT("qwer3"));
//	}
//}
void UJJH_GameInstance::ServerSetPlayerMesh_Implementation(APlayerController* PC, USkeletalMesh* NewMesh)
{
	if ( PC && NewMesh )
	{
		APlayerState* PlayerState = PC->GetPlayerState<APlayerState>();
		if ( PlayerState )
		{
			int32 PlayerId = PlayerState->GetPlayerId();
			UE_LOG(LogTemp, Log, TEXT("Player PlayerId: %d"), PlayerId);		
			PlayerMeshes.Add(PlayerId, NewMesh);
			ClientUpdatePlayerMesh(PlayerId, NewMesh);
		}
	}
}

void UJJH_GameInstance::ClientUpdatePlayerMesh_Implementation(int32 PlayerId, USkeletalMesh* NewMesh)
{
	UE_LOG(LogTemp, Error, TEXT("ClientUpdatePlayerMesh called for player ID: %d"), PlayerId);

	APlayerController* PlayerController = GetPlayerControllerFromUniqueID(PlayerId);
	if ( PlayerController )
	{
		ISelectCharacterInterface* UpdateablePawn = Cast<ISelectCharacterInterface>(PlayerController->GetPawn());
		if ( UpdateablePawn )
		{
			UpdateablePawn->UpdateMesh(NewMesh);
			UE_LOG(LogTemp, Error, TEXT("Mesh updated for player ID: %d"), PlayerId);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerController not found for ID: %d"), PlayerId);
	}
}

void UJJH_GameInstance::MulticastSyncAllPlayerMeshes_Implementation()
{
	for ( const auto& Pair : PlayerMeshes )
	{
		ClientUpdatePlayerMesh(Pair.Key, Pair.Value);
		UE_LOG(LogTemp, Error, TEXT("Syncing mesh for player ID: %d"), Pair.Key);
	}
}

void UJJH_GameInstance::SyncAllPlayerMeshes()
{
	UE_LOG(LogTemp, Error, TEXT("qwer"));
	MulticastSyncAllPlayerMeshes();
}

APlayerController* UJJH_GameInstance::GetPlayerControllerFromUniqueID(int32 UniqueID)
{
	for ( FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It )
	{
		APlayerController* PC = It->Get();
		if ( IsValid(PC) )
		{	
			APlayerState* PlayerState = PC->GetPlayerState<APlayerState>();
			if ( PlayerState )
			{
				int32 CurrentID = PlayerState->GetPlayerId();
				UE_LOG(LogTemp, Log, TEXT("Checking PC: %d, Passed UniqueID: %d"), CurrentID, UniqueID);
				if ( CurrentID == UniqueID )
				{
					return PC;
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Invalid PlayerController encountered"));
		}
	}
	return nullptr;
}
