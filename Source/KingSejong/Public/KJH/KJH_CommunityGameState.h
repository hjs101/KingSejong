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
	UFUNCTION(BlueprintCallable)
	void OnChangedTeacherMessage(const FString& Message);
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerRPC_OnChangedTeacherMessage(const FString& Message);

	//UFUNCTION(NetMulticast, Reliable)
	//void MulticastRPC_OnChangedTeacherMessage(const FString& Message);


	UFUNCTION(BlueprintCallable)
	void OnChangedTeacherAudioId(const FString& AudioId);
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerRPC_OnChangedTeacherAudioId(const FString& AudioId);
	//UFUNCTION(NetMulticast, Reliable)
	//void MulticastRPC_OnChangedTeacherAudioId(const FString& AudioId);
};
