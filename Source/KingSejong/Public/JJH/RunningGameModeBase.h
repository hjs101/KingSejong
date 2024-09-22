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

	TArray<class ARunnerController*> FailedToFinishPlayers;  // 플레이어 컨트롤러를 저장할 배열

	//퀴즈 데이타 뽑기
	UFUNCTION(NetMulticast, Reliable)
	void MulticastSendQuizData(const FWordsData& QuizData);

	FWordsData SelectedQuiz;

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

	//그냥 사람들 담을 배열
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<class ARunnerController*> Players;
	
	UFUNCTION(NetMulticast, Reliable)
	void MulticastStartCountdownTimer();


	FTimerHandle ShowAnswerTextBoxTimerHandle;
	void ShowAnswerUIToPlayerInOrder();

	UPROPERTY(EditAnywhere)
	float CountDownNum = 5;

	//입력 허가
	void AbleInput();
	//정답 입력 허가
	void SubmitAnswer();
	//다음 플레이어로 정답 입력 기회 넘기기
	void MoveToNextPlayer();

	//UFUNCTION(NetMulticast, Reliable)
	//void MuticastUpdateAnswerTextToAll();

	void UpdateTextInGameMode(const FString& AnswerText);
	

	void CheckAnswer(const FString& UserAnswer, ARunnerController* AnsweringPlayer);


	void EndGameAndReturnToLobby(ARunnerController* WinningPlayer);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastShowTeachSpeak(bool bIsCorrect);


	virtual void PostLogin(APlayerController* NewPlayer) override;

	int32 CurrentPlayerCount = 0;
	int32 RequiredPlayerCount = 2; // 원하는 플레이어 수 설정

	bool bQuizStarted = false;

	void CheckAndStartQuiz();


};
