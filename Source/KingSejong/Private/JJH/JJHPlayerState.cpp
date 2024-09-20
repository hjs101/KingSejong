// Fill out your copyright notice in the Description page of Project Settings.


#include "JJH/JJHPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "JJH/SelectPlayerInterface.h"

void AJJHPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(AJJHPlayerState, SelectedMesh);
}

void AJJHPlayerState::OnRep_SelectedMesh()
{
    APawn* Pawn = GetPawn();
    if ( Pawn && SelectedMesh )
    {
        ISelectPlayerInterface* UpdateablePawn = Cast<ISelectPlayerInterface>(Pawn);
        if ( UpdateablePawn )
        {
            UpdateablePawn->UpdateMesh(SelectedMesh);
        }
    }
}

void AJJHPlayerState::ServerSetSelectedMesh_Implementation(USkeletalMesh* NewMesh)
{
    SelectedMesh = NewMesh;
    OnRep_SelectedMesh();
}
