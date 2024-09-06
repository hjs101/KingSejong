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

	void MoveToNextPlayerWithDelay();
	void MoveToNextPlayer();
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UQuizWidget> QuizWidgetClass;

	class UQuizWidget* QuizWidgetInstance;

	//클라이언트 붙여줘야 컨트롤러 제대로 구분할 수 있는 듯??
	UFUNCTION(Client, Reliable)
	void ClientCreateQuizWidget(const FWordsData& QuizData);
	
	UFUNCTION(Client, Reliable)
	void ClientShowAnswerTextBox();

	UFUNCTION(Client, Reliable)
	void ClientStartWidgetCountDown();	
	
	UFUNCTION(Client , Reliable)
	void ClientDisableInput();	
	
	UFUNCTION(Client , Reliable)
	void ClientAbleInput();

	UFUNCTION(Client , Reliable)
	void ClientShowLoading();	
	
	UFUNCTION(Client , Reliable)
	void ClientSpectatePlayer(AActor* TargetPlayer);


	UFUNCTION(Server , Reliable)
	void ServerUpdateTextBoxContent(const FString& TextContent);

	UFUNCTION(NetMulticast , Reliable)
	void MulticastUpdateTextBoxContent(const FString& TextContent);

};
