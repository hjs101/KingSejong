// Fill out your copyright notice in the Description page of Project Settings.


#include "HJS/HJS_BattleGameMode.h"
#include "HJS/HJS_BattlePlayer.h"

AHJS_BattleGameMode::AHJS_BattleGameMode()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AHJS_BattleGameMode::BeginPlay()
{
    Super::BeginPlay();
    GetWorldTimerManager().SetTimer(TestHandle,this,&AHJS_BattleGameMode::StartRecordingForAllPlayers,3.f,false);
}

void AHJS_BattleGameMode::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
}

void AHJS_BattleGameMode::StartRecordingForAllPlayers()
{
    // 게임에서 현재 존재하는 모든 플레이어 컨트롤러를 가져옵니다.
    int32 index = 0;
    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        // 플레이어 컨트롤러를 가져옵니다.
        APlayerController* PlayerController = It->Get();
        if (PlayerController)
        {
            // 플레이어 캐릭터를 가져옵니다.
            AHJS_BattlePlayer* BattlePlayer = Cast<AHJS_BattlePlayer>(PlayerController->GetCharacter());
            if (BattlePlayer)
            {
                // 플레이어의 녹음을 시작합니다.
                Client_StartRecording(PlayerController,index++);
            }
        }
    }
}

void AHJS_BattleGameMode::Client_StartRecording(APlayerController* PlayerController, int32 Index)
{
    AHJS_BattlePlayer* Character = Cast<AHJS_BattlePlayer>(PlayerController->GetCharacter());
    if (Character)
    {
        Character->StartRecording(Index);
    }
}
