// Fill out your copyright notice in the Description page of Project Settings.


#include "JJH/JJHPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "JJH/JJH_GameInstance.h"

void AJJHPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(AJJHPlayerState, SelectedMeshIndex);

}
