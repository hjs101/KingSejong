﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KJH/KJH_InteractiveActor.h"
#include "KJH_Chair.generated.h"

/**
 * 
 */
UCLASS()
class KINGSEJONG_API AKJH_Chair : public AKJH_InteractiveActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AKJH_Chair();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UPROPERTY(Replicated)
	class AKJH_Player* TargetPlayer;

public:
	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* BoxComp;

	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* MeshComp;


	UPROPERTY(EditDefaultsOnly)
	class UArrowComponent* SitArrowComp;


public:
	virtual void OnBeginInteraction(class AActor* OtherActor) override;
	virtual void OnEndInteraction() override;
	virtual bool IsInteractable() override;


public:

	// 앉기
	UFUNCTION(Server, Reliable)
	void ServerSitDown(class AActor* OtherActor);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastSitDown(class AActor* OtherActor);


	// 일어나기
	UFUNCTION(Server, Reliable)
	void ServerStandUp();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastStandUp();


	void DebugLog(FString Message);
};
