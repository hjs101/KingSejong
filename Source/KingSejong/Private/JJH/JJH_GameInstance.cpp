// Fill out your copyright notice in the Description page of Project Settings.


#include "JJH/JJH_GameInstance.h"
#include "OnlineSubSystem.h"
#include "../../../../Plugins/Online/OnlineSubsystem/Source/Public/Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"

#include "JJH/LobbyWidget.h"

const static FName SESSION_NAME = TEXT("My Session Game");

UJJH_GameInstance::UJJH_GameInstance()
{
	
}

void UJJH_GameInstance::Init()
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem)
	{
		SessionInterface = Subsystem->GetSessionInterface();
		if ( SessionInterface )
		{
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UJJH_GameInstance::OnCreateSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UJJH_GameInstance::OnDestroySessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UJJH_GameInstance::OnFindSessionComplete);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Found no Subsystem"));
	}

}

void UJJH_GameInstance::LoadMenu()
{
	Lobby = CreateWidget<ULobbyWidget>(GetWorld(), MenuClass);
	
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

void UJJH_GameInstance::Host()
{
	if ( SessionInterface.IsValid() )
	{
		auto ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);
		if ( ExistingSession != nullptr )
		{
			SessionInterface->DestroySession(SESSION_NAME);
		}
		else
		{
			CreateSession();
		}
	}
}

void UJJH_GameInstance::Join(const FString& Address)
{
	UEngine* Engine = GetEngine();
	if ( !ensure(Engine != nullptr) ) return;
	Engine->AddOnScreenDebugMessage(0, 5, FColor::Green, FString::Printf(TEXT("Joining %s"), *Address));
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if ( !ensure(PlayerController != nullptr) ) return;

	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}

void UJJH_GameInstance::CreateSession()
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
		//settings.Set(FName("Category"), FName("달리기"), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
		//settings.Set(FName("Room_Name"), roomName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
		//settings.Set(FName("Host_Name"), MySessionName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

		FUniqueNetIdPtr netID = GetWorld()->GetFirstLocalPlayerFromController()->GetUniqueNetIdForPlatformUser().GetUniqueNetId();
		SessionInterface->CreateSession(*netID, SESSION_NAME, SessionSettings);
	}
}

void UJJH_GameInstance::OnCreateSessionComplete(FName SessionName, bool Success)
{
	if ( !Success )
	{
		return;
	}

	GEngine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Hosting"));

	GetWorld()->ServerTravel("/Game/JJH/ProtoRunningMap?listen");
}

void UJJH_GameInstance::OnDestroySessionComplete(FName SessionName, bool Success)
{
	//파괴에 성공하면 다시 만들기
	if ( Success )
	{
		UE_LOG(LogTemp, Warning, TEXT("Session Destroyed"));
		CreateSession();
	}
}

void UJJH_GameInstance::OnFindSessionComplete(bool Success)
{
	
}

//void UJJH_GameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
//{

//}

