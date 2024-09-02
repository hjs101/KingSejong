// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HJS_BattleGameMode.generated.h"

/**
 * 
 */
UCLASS()
class KINGSEJONG_API AHJS_BattleGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	// Sets default values for this character's properties
	AHJS_BattleGameMode();

protected:
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;
public:
	void StartRecordingForAllPlayers();

	FTimerHandle TestHandle;

	// 클라이언트에서 실행되는 함수
    UFUNCTION()
    void Client_StartRecording(APlayerController* PlayerController, int32 Index);

    //UFUNCTION(Client, Reliable)
    //void Client_StopRecording(APlayerController* PlayerController);

};
