﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "KJH_PlayerController.generated.h"

UCLASS()
class KINGSEJONG_API AKJH_PlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	AKJH_PlayerController();

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UUserWidget> QuizWidgetFactory;

	UPROPERTY()
	class UKJH_OXQuizWidget* QuizWidget;

	UPROPERTY(EditDefaultsOnly)
	class UKJH_QuizSoundHandler* SoundHandler;

public:
	UPROPERTY(EditDefaultsOnly, Category=Sound)
	class USoundBase* SFX_Correct;

	UPROPERTY(EditDefaultsOnly, Category = Sound)
	class USoundBase* SFX_Fail;

	UPROPERTY(EditDefaultsOnly, Category = Sound)
	class USoundBase* SFX_Tick;

	UPROPERTY(EditDefaultsOnly, Category = Sound)
	class USoundBase* SFX_Buzzer;

	UPROPERTY(EditDefaultsOnly, Category = Sound)
	class USoundBase* SFX_Notification;

	UPROPERTY(EditDefaultsOnly, Category = Sound)
	class USoundBase* SFX_Bright;

	UPROPERTY(EditDefaultsOnly, Category = Sound)
	class UAudioComponent* AudioComp;

	UPROPERTY(EditDefaultsOnly, Category = CameraShake)
	TSubclassOf<class UCameraShakeBase> CameraShake;


public:

	UFUNCTION(Client, Reliable)
	void ClientRPC_InitializeQuiz(int32 QuizTime);

	UFUNCTION(Client, Reliable)
	void ClientRPC_ShowQuestionWidget(const FString& Question);

	UFUNCTION(Client, Reliable)
	void ClientRPC_ShowWaitingWidget();

	UFUNCTION(Client, Reliable)
	void ClientRPC_ShowWidgetAnswer(bool bCorrectAnswer, bool bSelectedAnswer);

	UFUNCTION(Client, Reliable)
	void ClientRPC_EndQuiz();

private:
	bool IsValidQuizWidget();

	void PlayQuizSound(USoundBase* Sound);
};
