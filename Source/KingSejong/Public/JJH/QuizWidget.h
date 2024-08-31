// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JJH/QuizData.h"
#include "QuizWidget.generated.h"


/**
 * 
 */
UCLASS()
class KINGSEJONG_API UQuizWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Initials;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Meaning;	
	
	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* AnswerTextBox;

	void InitializeQuiz(const FWordsData& WordData);

	FTimerHandle ShowInitialTimerHandle;
	FWordsData CurrentWordData;
	
	void ShowInitials();
	void ShowAnswerTextBox();
};
