// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "KJH_CommunityGameMode.generated.h"

UENUM(BlueprintType)
enum class ECommunityMode :uint8
{
	Community,
	Quiz
};

UCLASS()
class KINGSEJONG_API AKJH_CommunityGameMode : public AGameMode
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;


public:
	UPROPERTY(BlueprintReadOnly)
	TArray<class AKJH_PlayerController*> PlayerControllers;

	UPROPERTY(BlueprintReadWrite)
	ECommunityMode CommunityMode;

public:
	UFUNCTION(BlueprintCallable)
	void GetAllPlayerControllers();

	//UFUNCTION(BlueprintCallable)
	//void SetCommunityState(ECommunityState NewState);

	UFUNCTION(BlueprintCallable)
	void PlayQuizMode();

	//void SetCommunityState(ECommunityState NewState);
	//void SetAllPlayersPosition(FTransform& TargetTransform);

};
