// Fill out your copyright notice in the Description page of Project Settings.


#include "JJH/RunnerAnimInstance.h"
#include "JJH/Runner.h"

void URunnerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Me = Cast<ARunner>(GetOwningActor());
}

void URunnerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if ( Me )
	{
		bAnimCanDance = Me->bCanDance;
	}
}


