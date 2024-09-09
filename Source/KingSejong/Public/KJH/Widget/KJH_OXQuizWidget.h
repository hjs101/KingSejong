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
	AnswerTrue,
	AnswerFalse,
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
	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* WidgetSwitcher_Quiz;
	UPROPERTY(meta = (BindWidget))
	class UMultiLineEditableTextBox* Text_Question;


	// Answer
	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* WidgetSwitcher_Answer;
	UPROPERTY(meta = (BindWidget))
	class UOverlay* Overlay_Answer;

	// Timer
	UPROPERTY(meta = (BindWidget))
	class UOverlay* Overlay_Timer;
	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* Text_Timer;

	UPROPERTY(BlueprintReadOnly)
	int32 CurrScore = 0;

private:
	// 타이머 핸들
	FTimerHandle TimerHandle;
	// 남은 시간을 저장할 변수
	int32 RemainingTime;
	int32 MaxQuizTime;

	//// 현재 문제 정답
	//bool bCurrAnswer;

public:
	EQuizWidgetState QuizState = EQuizWidgetState::Idle;

public:

	UFUNCTION(BlueprintImplementableEvent)
	void EndState();

	// Quiz 상태
	UFUNCTION(BlueprintCallable)
	void SetQuizWidgetState(EQuizWidgetState State);

	void IdleState(int32 QuizTime);
	void QuestionState(FString Quistion);
	void WaitingState();
	void AnswerState(bool bCorrectAnswer, bool bSelectedAnswer);

	// 정답 여부
	UFUNCTION(BlueprintCallable)
	void SetActiveAnswerStatus(bool bActive, bool bIsCorrect = false);

private:
	// 타이머
	void ShowTimer(bool bValue);
	void StartTimer();
	void UpdateTimer();


};
