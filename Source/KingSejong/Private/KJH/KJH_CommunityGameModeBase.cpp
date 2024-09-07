// Fill out your copyright notice in the Description page of Project Settings.


#include "KJH/KJH_CommunityGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "KJH/KJH_Player.h"

void AKJH_CommunityGameModeBase::BeginPlay()
{
    Super::BeginPlay();

    // PlayStart 위치
    APlayerStart* playerStart = Cast<APlayerStart>(UGameplayStatics::GetActorOfClass(GetWorld(), APlayerStart::StaticClass()));

     if ( playerStart )
     {
         PlayerStartTr = playerStart->GetActorTransform();
     }
     
    // Quiz 위치
    TArray<AActor*> outActors;
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("QuizStartPoint"), outActors);

    if ( outActors.Num() > 0 )
    {
        QuizStartTr = outActors[0]->GetActorTransform();
    }

    //GetAllPlayers();
    //SetCommunityState(ECommunityState::Quiz);

    //FTimerHandle timerHandle;
    //GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &AKJH_CommunityGameModeBase::SetAllPlayersPosition, 10, false);
}

void AKJH_CommunityGameModeBase::GetAllPlayers()
{
    TArray<AActor*> players;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AKJH_Player::StaticClass(), players);

    for(auto player : players)
    {
        auto* p = Cast<AKJH_Player>(player);
        if ( p )
        {
            PlayerList.Add(p);
        }
    }

    //UE_LOG(LogTemp, Warning, TEXT("GetAllPlayers Call!!"));
}

void AKJH_CommunityGameModeBase::SetCommunityState(ECommunityState NewState)
{
    if(CommunityState == NewState) return;

    CommunityState = NewState;

    FTransform TargetTr = CommunityState == ECommunityState::Debate ? PlayerStartTr : QuizStartTr;

    SetAllPlayersPosition(TargetTr);

    UE_LOG(LogTemp, Warning, TEXT("CommunityState : %d"), CommunityState);
}

void AKJH_CommunityGameModeBase::SetAllPlayersPosition(FTransform& TargetTransform)
{
    GetAllPlayers();

    if(PlayerList.Num() <= 0) return;

    for ( auto player : PlayerList)
    {
        if(player == nullptr) continue;

        player->SetPlayerPosition(TargetTransform);
    }
}