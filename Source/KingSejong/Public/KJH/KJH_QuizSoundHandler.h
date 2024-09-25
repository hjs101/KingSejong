// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "KJH_QuizSoundHandler.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class KINGSEJONG_API UKJH_QuizSoundHandler : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UKJH_QuizSoundHandler();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


public:
	UPROPERTY(EditDefaultsOnly)
	class AKJH_PlayerController* OwnerPC;


	UPROPERTY(EditDefaultsOnly)
	class USoundBase* SFX_Correct;

	UPROPERTY(EditDefaultsOnly)
	class USoundBase* SFX_Fail;
	
	UPROPERTY(EditDefaultsOnly)
	class USoundBase* SFX_BGM;

	UPROPERTY(EditDefaultsOnly)
	class USoundBase* SFX_Tick;

	UPROPERTY(EditDefaultsOnly)
	class USoundBase* SFX_Buzzer;

	UPROPERTY(EditDefaultsOnly)
	class USoundBase* SFX_Notification;

	UPROPERTY(EditDefaultsOnly)
	class USoundBase* SFX_Bright;


public:
	void PlayQuizSound(class USoundBase* Sound);
};
