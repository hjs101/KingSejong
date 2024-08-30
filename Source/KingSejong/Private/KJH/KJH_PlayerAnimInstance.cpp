// Fill out your copyright notice in the Description page of Project Settings.


#include "KJH/KJH_PlayerAnimInstance.h"
#include "KJH/KJH_Player.h"
#include "GameFramework/CharacterMovementComponent.h"

void UKJH_PlayerAnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();

    auto owner = TryGetPawnOwner();
    Player = Cast<AKJH_Player>(owner);
}

void UKJH_PlayerAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
    if (Player != nullptr)
    {
        FVector velocity = Player->GetVelocity();
        FVector forwordVector = Player->GetActorForwardVector();
        Speed = FVector::DotProduct(forwordVector, velocity);

        auto* movement = Player->GetCharacterMovement();
        bIsJump = movement->IsFalling();
    }
}

void UKJH_PlayerAnimInstance::SetSitState(bool bValue)
{
    bIsSit = bValue;
}
