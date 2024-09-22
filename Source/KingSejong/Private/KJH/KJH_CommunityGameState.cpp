// Fill out your copyright notice in the Description page of Project Settings.


#include "KJH/KJH_CommunityGameState.h"
#include "Net/UnrealNetwork.h"

void AKJH_CommunityGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AKJH_CommunityGameState, TeacherMessage);
    DOREPLIFETIME(AKJH_CommunityGameState, TeacherAudioId);
}

void AKJH_CommunityGameState::OnChangedTeacherMessage(const FString& Message)
{
    if ( HasAuthority() )
    {
        TeacherMessage = Message;
    }
    else
    {
        ServerRPC_OnChangedTeacherMessage(Message);
    }
}

void AKJH_CommunityGameState::ServerRPC_OnChangedTeacherMessage_Implementation(const FString& Message)
{
    TeacherMessage = Message;
}

//void AKJH_CommunityGameState::MulticastRPC_OnChangedTeacherMessage_Implementation(const FString& Message)
//{
//    TeacherMessage = Message;
//}

void AKJH_CommunityGameState::OnChangedTeacherAudioId(const FString& AudioId)
{
    if ( HasAuthority() )
    {
        TeacherAudioId = AudioId;
    }
    else
    {
        ServerRPC_OnChangedTeacherMessage(AudioId);
    }
}

void AKJH_CommunityGameState::ServerRPC_OnChangedTeacherAudioId_Implementation(const FString& AudioId)
{
    TeacherAudioId = AudioId;
}

//void AKJH_CommunityGameState::MulticastRPC_OnChangedTeacherAudioId_Implementation(const FString& AudioId)
//{
//    MulticastRPC_OnChangedTeacherAudioId(AudioId);
//    TeacherAudioId = AudioId;
//}