﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "LobbyInterface.h"
#include "../../../../Plugins/Online/OnlineSubsystem/Source/Public/Interfaces/OnlineSessionInterface.h"
#include "JJH_GameInstance.generated.h"

UENUM(BlueprintType)
enum class ERoomCategory : uint8
{
    Run     UMETA(DisplayName = "Run"),
    Battle  UMETA(DisplayName = "Battle"),
    Talk    UMETA(DisplayName = "Talk")
};

USTRUCT(BlueprintType)
struct FRoomInfo {
	GENERATED_BODY()
	
     //방 카테고리를 Enum으로 묶기
     UPROPERTY(BlueprintReadOnly)
     ERoomCategory RoomCategory;

	 //방이름
	 UPROPERTY(BlueprintReadOnly)
	 FString roomName;
	 UPROPERTY(BlueprintReadOnly)
	 FString hostName;
	 UPROPERTY(BlueprintReadOnly)
	 int32 MaxPlayerCount;
	 UPROPERTY(BlueprintReadOnly)
	 int32 CurrentPlayerCount;
	 UPROPERTY(BlueprintReadOnly)
	 int32 pingMS;

	 int32 index;

	 FString ToString()
	 {
		 return FString::Printf(TEXT(" %d) [%s] [%s]( %d / %d)-> %dms"), index, *roomName, *hostName, CurrentPlayerCount, MaxPlayerCount, pingMS);
	 }
};

//자료형과 변수를 ,로 구분해서 쓰기
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSearchSignature, const struct FRoomInfo&, info);

/**
 * 
 */
UCLASS()
class KINGSEJONG_API UJJH_GameInstance : public UGameInstance, public ILobbyInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void LoadMenu();

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ULobbyWidget> MenuClass;

	UJJH_GameInstance();

	virtual void Init() override;

	IOnlineSessionPtr SessionInterface;

	//방생성
	void CreateSession(const FString& RoomName, int32 PlayerCount, const FString& Category) override;
	
	void FindOtherSessions();
	//방찾기
	void FindSession();

	//찾을 방의 목록
	TSharedPtr<FOnlineSessionSearch> SessionSearch;

	//방 이름
	FString MySessionName = TEXT("My Session");

	//방 조인하기
	virtual void JoinToSession(int32 Index) override;

	//방 부수기
	void DestroySession();
	//델리게이트
	void OnMyCreateSessionComplete(FName SessionName, bool Success);
	void OnMyDestroySessionComplete(FName SessionName, bool Success);
	void OnMyFindSessionComplete(bool Success);
	void OnMyJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	
	//델리게이트 걸기
	FSearchSignature OnSearchSignatureCompleteDelegate;

	FString CategoryValue;
	FString RoomNameValue;
	FString HostNameValue;

	UFUNCTION(BlueprintCallable, Category = "Character")
	void SetSelectedCharacterMesh(USkeletalMesh* Mesh);

	class USkeletalMesh* SelectedCharacterMesh;

	//=============================메시
	//UPROPERTY()
 //   USkeletalMesh* SelectedMesh;

 //   //UFUNCTION()
 //   //void OnRep_SelectedMesh();

	//void SetSelectedMesh(USkeletalMesh* NewMesh, APlayerController* PC);
 //   UFUNCTION(Server, Reliable)
 //   void ServerSetSelectedMesh(USkeletalMesh* NewMesh, APlayerController* PC);
	//UFUNCTION(NetMulticast, Reliable)
 //   void MulticastSetSelectedMesh(USkeletalMesh* NewMesh, APlayerController* PC);
	//
	
	void UpdatePlayerMesh(APlayerController* PC, USkeletalMesh* NewMesh);

};
