// Fill out your copyright notice in the Description page of Project Settings.


#include "KJH/KJH_PlayerQuizHandler.h"

// Sets default values for this component's properties
UKJH_PlayerQuizHandler::UKJH_PlayerQuizHandler()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UKJH_PlayerQuizHandler::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UKJH_PlayerQuizHandler::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

