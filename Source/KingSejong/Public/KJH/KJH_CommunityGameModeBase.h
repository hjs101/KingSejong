// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "KJH_CommunityGameModeBase.generated.h"

UENUM()
enum class ECommunityState :uint8
{
	Debate,
	Quiz
};

UCLASS()
class KINGSEJONG_API AKJH_CommunityGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY()
	TArray<class AKJH_Player*> PlayerList;

public:
	FTransform PlayerStartTr;
	FTransform QuizStartTr;
	ECommunityState CommunityState;

public:
	void GetAllPlayers();


	UFUNCTION(BlueprintCallable)
	void SetCommunityState(ECommunityState NewState);
	void SetAllPlayersPosition(FTransform& TargetTransform);
	
};
