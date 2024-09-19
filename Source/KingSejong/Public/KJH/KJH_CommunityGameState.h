// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "KJH_CommunityGameState.generated.h"

/**
 * 
 */
UCLASS()
class KINGSEJONG_API AKJH_CommunityGameState : public AGameState
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Replicated)
	FString TeacherMessage;
	UPROPERTY(Replicated)
	FString TeacherAudioId;

public:
	void OnChangedTeacherMessage(const FString& Message);
	UFUNCTION(Server, Reliable)
	void ServerRPC_OnChangedTeacherMessage(const FString& Message);

	void OnChangedTeacherAudioId(const FString& AudioId);
	UFUNCTION(Server, Reliable)
	void ServerRPC_OnChangedTeacherAudioId(const FString& AudioId);
};
