// Fill out your copyright notice in the Description page of Project Settings.


#include "KJH/KJH_InteractiveActor.h"

// Sets default values
AKJH_InteractiveActor::AKJH_InteractiveActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AKJH_InteractiveActor::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AKJH_InteractiveActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AKJH_InteractiveActor::OnBeginInteraction(AActor* OtherActor)
{
	if(OnBeginInteractionDelegate.IsBound())
	{
		OnBeginInteractionDelegate.Broadcast(OtherActor);
	}
}

void AKJH_InteractiveActor::OnEndInteraction()
{
	if (OnEndInteractionDelegate.IsBound())
	{
		OnEndInteractionDelegate.Broadcast();
	}
}

bool AKJH_InteractiveActor::IsInteractable()
{
	return true;
}

