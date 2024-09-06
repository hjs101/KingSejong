// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KJH_InteractiveActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBeginInteractionSignature, class AActor*, OtherActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEndInteractionSignature);

UCLASS()
class KINGSEJONG_API AKJH_InteractiveActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AKJH_InteractiveActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(BlueprintAssignable)
	FBeginInteractionSignature OnBeginInteractionDelegate;

	UPROPERTY(BlueprintAssignable)
	FEndInteractionSignature OnEndInteractionDelegate;

	UFUNCTION()
	virtual void OnBeginInteraction(class AActor* OtherActor);
	UFUNCTION()
	virtual void OnEndInteraction();

	virtual bool IsInteractable();

};
