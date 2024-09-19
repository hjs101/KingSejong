// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "KJH_PlayerController.generated.h"

UCLASS()
class KINGSEJONG_API AKJH_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UUserWidget> QuizWidgetFactory;

	UPROPERTY()
	class UKJH_OXQuizWidget* QuizWidget;

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
};
