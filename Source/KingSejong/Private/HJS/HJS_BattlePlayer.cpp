// Fill out your copyright notice in the Description page of Project Settings.


#include "HJS/HJS_BattlePlayer.h"

// Sets default values
AHJS_BattlePlayer::AHJS_BattlePlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AHJS_BattlePlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHJS_BattlePlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AHJS_BattlePlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AHJS_BattlePlayer::StartRecording()
{
}

void AHJS_BattlePlayer::StopRecording()
{
}

void AHJS_BattlePlayer::StartRecordingImplementation()
{
}

void AHJS_BattlePlayer::StopRecordingImplementation()
{
}

