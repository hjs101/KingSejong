// Fill out your copyright notice in the Description page of Project Settings.


#include "KJH/KJH_QuizSoundHandler.h"
#include "KJH/KJH_PlayerController.h"
#include "Components/AudioComponent.h"

// Sets default values for this component's properties
UKJH_QuizSoundHandler::UKJH_QuizSoundHandler()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UKJH_QuizSoundHandler::BeginPlay()
{
	Super::BeginPlay();

	OwnerPC = Cast<AKJH_PlayerController>(GetOwner());

}


// Called every frame
void UKJH_QuizSoundHandler::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UKJH_QuizSoundHandler::PlayQuizSound(USoundBase* Sound)
{
	if (OwnerPC && OwnerPC->AudioComp)
	{
		//OwnerPC->ClientPlaySound(Sound);
		OwnerPC->AudioComp->Stop();

		OwnerPC->AudioComp->Sound = Sound;
		OwnerPC->AudioComp->Play();
	}
}

