// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BattlePlayerAnim.generated.h"

/**
 * 
 */
UCLASS()
class KINGSEJONG_API UBattlePlayerAnim : public UAnimInstance
{
	GENERATED_BODY()
	


public:
	
	UPROPERTY(EditAnywhere)
	class UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* HitMontage;

	UFUNCTION()
	void PlayHitMontage();

	UFUNCTION()
	void PlayChargingMontage();

	UFUNCTION()
	void JumpMontageToAttack();

	
	UPROPERTY(BlueprintReadWrite)
	bool bAttack = false;

	UPROPERTY(BlueprintReadWrite)
	bool bCharging = false;
	UPROPERTY(BlueprintReadWrite)
	bool bDie = false;
	UFUNCTION()
	void AnimNotify_ToCharging();

	UFUNCTION()
	void AnimNotify_AttackEnd();

	UFUNCTION()
	void AnimNotify_AttackPoint();

	UFUNCTION()
	void AnimNotify_HitEnd();

	UFUNCTION()
	void PlayReturnAnim();

	UFUNCTION()
	void AnimNotify_ReturnEnd();

	UPROPERTY(BlueprintReadWrite)
	bool bReturn = false;

};
