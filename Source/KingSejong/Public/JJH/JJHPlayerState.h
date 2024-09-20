// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "JJHPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class KINGSEJONG_API AJJHPlayerState : public APlayerState
{
	GENERATED_BODY()
public:

	UPROPERTY(ReplicatedUsing = OnRep_SelectedMesh)
    class USkeletalMesh* SelectedMesh;

    UFUNCTION()
    void OnRep_SelectedMesh();

    UFUNCTION(Server, Reliable)
    void ServerSetSelectedMesh(USkeletalMesh* NewMesh);
};
