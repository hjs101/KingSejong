// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "JJH/QuizData.h"
#include "RunningGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class KINGSEJONG_API ARunningGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	
	virtual void BeginPlay() override;
	//퀴즈 위젯
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UQuizWidget> QuizWidgetClass;

	//문제 리스트
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UDataTable* WordsDataTable;

	//초성퀴즈 문제고르기
	FWordsData SelectRandomQuizData();

	//서버측에서 문제 뿌리기
	UFUNCTION(BlueprintCallable)
	void StartQuiz();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastSendQuizData(const FWordsData& QuizData);


	void PlayerCrossedFinishLine(APlayerController* PlayerController);
	
	TArray<APlayerController*> PlayerFinishOrder;
	
	UFUNCTION(NetMulticast, Reliable)
	void MulticastStartCountdownTimer();
};
