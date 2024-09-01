// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "JJH/QuizData.h"
#include "RunnerController.generated.h"

/**
 * 
 */
UCLASS()
class KINGSEJONG_API ARunnerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UQuizWidget> QuizWidgetClass;

	class UQuizWidget* QuizWidgetInstance;

	//클라이언트 붙여줘야 컨트롤러 제대로 구분할 수 있는 듯??
	UFUNCTION(Client, Reliable)
	void ClientCreateQuizWidget(const FWordsData& QuizData);
	
	void ShowAnswerTextBox();

	UFUNCTION(Client, Reliable)
	void ClientStartWidgetCountDown();
};
