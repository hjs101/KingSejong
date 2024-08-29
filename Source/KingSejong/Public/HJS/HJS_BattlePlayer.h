// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HJS_BattlePlayer.generated.h"

UCLASS()
class KINGSEJONG_API AHJS_BattlePlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AHJS_BattlePlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


public:
	
	   // 녹음 시작
    UFUNCTION(BlueprintCallable, Server, Reliable)
    void StartRecording();

    // 녹음 종료
    UFUNCTION(BlueprintCallable, Server, Reliable)
    void StopRecording();



private:
	bool bIsRecording = false;

	void StartRecordingImplementation();
	void StopRecordingImplementation();
};
