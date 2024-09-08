// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "KJH/KJH_QuizManager.h"
#include "KJH_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class KINGSEJONG_API AKJH_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	//TSubclassOf<UUserWidget*>

public:
	void SetQuizWidgetState(EQuizState State);

	void SetQuizInfo();
	
	void ShowWidgetWaiting();

	void ShowWidgetAnswer(bool bCorrectAnswer, bool bSelectedAnswer);

	void EndQuiz();
};
