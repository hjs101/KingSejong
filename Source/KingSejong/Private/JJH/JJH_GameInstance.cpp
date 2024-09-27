﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "JJH/JJH_GameInstance.h"
#include "OnlineSubSystem.h"
#include "../../../../Plugins/Online/OnlineSubsystem/Source/Public/Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "../../../../Plugins/Online/OnlineBase/Source/Public/Online/OnlineSessionNames.h"
#include "Online/CoreOnline.h"
#include "JJH/LobbyWidget.h"
#include "string"

const static FName SESSION_NAME = TEXT("문해력 Zㅣ존");
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
	UE_LOG(LogTemp, Warning, TEXT("Joining"));
	auto result = SessionSearch->SearchResults[Index];
	SessionInterface->JoinSession(0, FName(SESSION_NAME), result);
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
		FString roomname = StringBase64Encode(RoomName);
		FString sessionname = StringBase64Encode(SESSION_NAME.ToString());
		SessionSettings.Set<FString>(FName("Category"), Category, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
		SessionSettings.Set<FString>(FName("Room_Name"), roomname, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
		SessionSettings.Set<FString>(FName("Host_Name"), sessionname, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

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
		GetWorld()->ServerTravel("/Game/JJH/BetaRunningMap?listen");
	}
	else if ( CategoryValue == TEXT("Battle") )
	{
		GetWorld()->ServerTravel("/Game/HJS/Maps/AlphaBattleMap?listen");
	}
	else if ( CategoryValue == TEXT("Talk") )
	{
		GetWorld()->ServerTravel("/Game/KJH/Maps/KJH_CommunityMap?listen");
	}
	GEngine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Hosting"));

}

void UJJH_GameInstance::FindOtherSessions()
{

	UE_LOG(LogTemp, Warning, TEXT("HName"));

	SessionSearch = MakeShareable(new FOnlineSessionSearch);

	//무슨조건으로 찾을래
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
	//랜인가?
	SessionSearch->bIsLanQuery = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL";
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
			roomInfo.roomName = StringBase64Decode(roomNameString);
			//호스트 이름
			FString hostNameString;
			results[ i ].Session.SessionSettings.Get<FString>(FName("Host_Name"), hostNameString);
			roomInfo.hostName = StringBase64Decode(hostNameString);
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
		EmptyRoomInfo.roomName = TEXT("입장 가능한 방이 없습니다.");
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

void UJJH_GameInstance::SetCharacterMesh(USkeletalMesh* Mesh)
{
	CharacterMesh = Mesh;
}

void UJJH_GameInstance::ExitSession()
{
	if (GetWorld()->IsNetMode(NM_Client))
	{
		ClientLeaveSession();
	}
	else
	{
		ServerRPCExitSession();
	}
}

void UJJH_GameInstance::ServerRPCExitSession_Implementation()
{
	SessionInterface->DestroySession(SESSION_NAME);
	MulticastRPCExitSession();
}

void UJJH_GameInstance::MulticastRPCExitSession_Implementation()
{
	if (nullptr != GEngine)
	{
		GEngine->OnNetworkFailure().AddUObject(this, &UJJH_GameInstance::OnNetworkFailure);
	}
	//방퇴장 요청
	//클라이언트 입장에서는 그냥 나가는 거 호스트 입장에선 파괴하는거
	//SessionInterface->DestroySession(SESSION_NAME);

	// 모든 클라이언트에게 세션이 종료되었음을 알림
	if (GetWorld()->IsNetMode(NM_Client))
	{
		// 클라이언트에서 로컬 정리 작업
		ClientLeaveSession();
	}

	UE_LOG(LogTemp, Error, TEXT("Session Destroy"));
}

void UJJH_GameInstance::ClientLeaveSession_Implementation()
{
	if (SessionInterface.IsValid())
	{
		SessionInterface->DestroySession(SESSION_NAME);
	}

	// 로비로 이동
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		PC->ClientTravel("/Game/JJH/MAP_Reallobby_SHN", TRAVEL_Absolute);
	}
}

void UJJH_GameInstance::OnMyDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		//로비로 여행을 가고 싶다.
		auto* pc = GetWorld()->GetFirstPlayerController();
		pc->ClientTravel(TEXT("/Game/JJH/MAP_Reallobby_SHN"), ETravelType::TRAVEL_Absolute);
	}
}

void UJJH_GameInstance::OnNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString)
{
	LoadServerWidgetMap();
}

void UJJH_GameInstance::LoadServerWidgetMap()
{
	// AKJH_PlayerController를 가져온다,
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	//// 플레이어의 첫번째 컨트롤러를 가져온다.
	//APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (PC && PC->IsLocalController()) // 컨트롤러가 있으면,
	{
		// ServerUI가 있는 맵으로 이동시킨다.
		PC->ClientTravel("/Game/JJH/MAP_Reallobby_SHN", ETravelType::TRAVEL_Absolute);
		UE_LOG(LogTemp, Error, TEXT("Session Destroy Network Failure"));
	}
}
	//============================ 스팀에서 한글명 방제 만들기
	// 보낼 때
FString UJJH_GameInstance::StringBase64Encode(const FString & str)
{
	// Set 할 때 : FString -> UTF8 (std::string) -> TArray<uint8> -> base64 로 Encode
	std::string utf8String = TCHAR_TO_UTF8(*str);
	TArray<uint8> arrayData = TArray<uint8>((uint8*)(utf8String.c_str()),
	utf8String.length());
	return FBase64::Encode(arrayData);
}

// 받을 때
FString UJJH_GameInstance::StringBase64Decode(const FString & str)
{
	// Get 할 때 : base64 로 Decode -> TArray<uint8> -> TCHAR
	TArray<uint8> arrayData;
	FBase64::Decode(str, arrayData);
	std::string ut8String((char*)(arrayData.GetData()), arrayData.Num());
	return UTF8_TO_TCHAR(ut8String.c_str());
	
	
}
