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

private:
	UPROPERTY()
	TArray<class AKJH_Player*> PlayerList;

public:
	UPROPERTY()
	FTransform PlayerStartTr;

	UPROPERTY()
	FTransform QuizStartTr;

	UPROPERTY()
	ECommunityState CommunityState;

private:
	void GetAllPlayers();

public:
	UFUNCTION(BlueprintCallable)
	void SetCommunityState(ECommunityState NewState);
	void SetAllPlayersPosition(FTransform& TargetTransform);
	
};
