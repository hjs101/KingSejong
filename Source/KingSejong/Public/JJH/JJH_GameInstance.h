// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
#include "LobbyInterface.h"
#include "JJH_GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class KINGSEJONG_API UJJH_GameInstance : public UGameInstance, public ILobbyInterface
{
	GENERATED_BODY()
public:

	UJJH_GameInstance();

	virtual void Init() override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ULobbyWidget> MenuClass;

	class ULobbyWidget* Lobby;
	UFUNCTION(BlueprintCallable)
	void LoadMenu();

	void Host();

	void Join(const FString& Address);

	//온라인 세션 인터페이스를 기억하고 싶다.
	IOnlineSessionPtr SessionInterface;

	//

	//방생성 요청
	void CreateSession();

	//방생성 델리게이트
	UFUNCTION()
	void OnCreateSessionComplete(FName SessionName, bool Success);

	//방 파괴 델리게이트
	UFUNCTION()
	void OnDestroySessionComplete(FName SessionName, bool Success);

	//방 조인 델리게이트
	UFUNCTION()
	void OnFindSessionComplete(bool Success);

	//플레이어 수
	int32 PlayerCount;


	//방 조인 델리게이트
	//UFUNCTION()
	//void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
};
