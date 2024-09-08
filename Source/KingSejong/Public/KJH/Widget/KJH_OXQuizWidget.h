// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KJH_OXQuizWidget.generated.h"

UENUM()
enum class EQuizWidgetState : uint8
{
	Idle,
	Question,
	Waiting,
	Answer,
	End
};

UCLASS()
class KINGSEJONG_API UKJH_OXQuizWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

public:
	// Quiz
	UPROPERTY(meta=(BindWidget))
	class UMultiLineEditableTextBox* Text_Quiz;

	// Answer
	UPROPERTY(meta=(BindWidget))
	class UWidgetSwitcher* WidgetSwitcher_QA;
	

	// Timer
	UPROPERTY(meta = (BindWidget))
	class UOverlay* Overlay_Timer;
	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* Text_Timer;

	UPROPERTY(EditDefaultsOnly)
	int32 IdleTime = 15;
	UPROPERTY(EditDefaultsOnly)
	int32 QuizTime = 30;
	UPROPERTY(EditDefaultsOnly)
	int32 WaitingTime = 5;
	UPROPERTY(EditDefaultsOnly)
	int32 AnwserTime = 10;

private:

	FTimerHandle TimerHandle;  // 타이머 핸들
	int32 RemainingTime;   // 남은 시간을 저장할 변수

	bool bCurrAnswer;

	UPROPERTY(EditDefaultsOnly)
	int32 MaxQuizCount = 3;
	int32 QuizCount = 0;
public:
	EQuizWidgetState QuizState = EQuizWidgetState::Idle;


public:
	void SetQuizState(EQuizWidgetState State);

	UFUNCTION(BlueprintImplementableEvent)
	void EndState();

private:
	// Quiz 상태
	void IdleState();
	void QuestionState();
	void WaitingState();
	void AnswerState();

	// 문제
	void SetQuizAndAnswer(FString Quistion, bool bAnswer);
	void GetQuiz();

	// 타이머
	void ShowTimer(bool bValue);
	void StartTimer();
	void UpdateTimer();


};
