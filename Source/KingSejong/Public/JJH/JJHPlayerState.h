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
    UPROPERTY(Replicated)
    int32 SelectedMeshIndex;

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};

