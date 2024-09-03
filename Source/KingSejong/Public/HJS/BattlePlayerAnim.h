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


	UFUNCTION()
	void PlayChargingMontage();

	UFUNCTION()
	void JumpMontageToAttack();


	UPROPERTY(BlueprintReadWrite)
	bool bAttack = false;

	UFUNCTION()
	void AnimNotify_ToCharging();

};
