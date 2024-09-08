// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "RunnerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class KINGSEJONG_API URunnerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	virtual void NativeInitializeAnimation() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAnimCanDance = false;

	class ARunner* Me;
};
