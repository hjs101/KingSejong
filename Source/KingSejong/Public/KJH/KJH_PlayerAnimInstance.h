// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "KJH_PlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class KINGSEJONG_API UKJH_PlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

private:
	UPROPERTY()
	class AKJH_Player* Player;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Speed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bIsSit;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bIsJump;

public:
	void SetSitState(bool bValue);
	
};
