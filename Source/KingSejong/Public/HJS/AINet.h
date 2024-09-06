// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AINet.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class KINGSEJONG_API UAINet : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAINet();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	// 서버 URL
	const FString ServerURL = "http://meta-ai.iptime.org:57347/hoonjang_stt";
		
};
