// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KJH_OXQuizManager.generated.h"

UENUM()
enum class EHorizontalState : uint8
{
	Left,
	Right
};


UCLASS()
class KINGSEJONG_API AKJH_OXQuizManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AKJH_OXQuizManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(BlueprintReadOnly)
	class AKJH_CommunityGameModeBase* MyGameModeBase;



public:
	
	UFUNCTION(BlueprintCallable)
	void CheckAnswer();


	EHorizontalState GetPlayerHorizontalState(class AActor* Target);


};
