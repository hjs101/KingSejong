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

	TArray<class ARunnerController*> PlayerControllers;  // 플레이어 컨트롤러를 저장할 배열

	//퀴즈 데이타 뽑기
	UFUNCTION(NetMulticast, Reliable)
	void MulticastSendQuizData(const FWordsData& QuizData);


	UFUNCTION(NetMulticast, Reliable)
	void MulticastMoveToNextLevel();

	//결승선 통과시
	void PlayerCrossedFinishLine(ARunnerController* PlayerController);
	
	void MoveToNextLevel();

	//로딩창 보여주기
	UFUNCTION(NetMulticast, Reliable)
	void MulticastShowLoading();

	//결승선 통과한 사람들 담을 배열
	TArray<class ARunnerController*> PlayerFinishOrder;
	
	UFUNCTION(NetMulticast, Reliable)
	void MulticastStartCountdownTimer();


	FTimerHandle ShowAnswerTextBoxTimerHandle;
	void ShowAnswerUIToPlayerInOrder();

	UPROPERTY(EditAnywhere)
	float CountDownNum = 5;

	void AbleInput();

	void SubmitAnswer();

};
