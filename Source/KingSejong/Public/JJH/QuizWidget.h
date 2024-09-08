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
	//처음에 모든 플레이어에게 뜻, 초성 보여주기
	//한명이 들어가면 모든 플레이어에게 카운트다운텍스트 하나씩 줄어드는거 보여주기
	//카운트다운 끝나면 결승선 통과한 사람들만 정답 입력칸 뜨기
	//통과 못했으면 실패위젯 띄우기
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Initials;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Meaning;	
	
	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* AnswerTextBox;	
	
	UPROPERTY(meta = (BindWidget))
	class UButton* AnswerSubmitButton;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CountDownText;
	
	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* QuizLoadingSwitcher;

	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* Quiz;	
	
	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* TeacherSpeak;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TeacherText;	
	
	UPROPERTY(meta = (BindWidget))
	class UImage* Teacher;	
	
	UPROPERTY(meta = (BindWidget))
	class UHorizontalBox* AnswerHorizontal;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* TeacherIntro;	
	
	UPROPERTY(meta = (BindWidgetAnim) , Transient)
	class UWidgetAnimation* TeacherAngry;
	
	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* QuizLoading;


	class UTexture2D* SmileTeacher = nullptr;;
	class UTexture2D* AngryTeacher = nullptr;;


	UPROPERTY(EditAnywhere)
	float CountDownNum;

	void InitializeQuiz(const FWordsData& WordData);

	FTimerHandle ShowInitialTimerHandle;
	FWordsData CurrentWordData;
	
	void ShowInitials();
	void ShowAnswerTextBox();

	void HideLoading();
	void ShowLoading();
	FTimerHandle CountDownTimerHandle;
	void StartCountDown();
	void UpdateCountDown();

	virtual void NativeConstruct() override;

	UFUNCTION()
	void SubmitAnswer();

	FTimerHandle loadingTimer;

	UFUNCTION()
	void OnAnswerTextChanged(const FText& Text);
	void UpdateTextBoxContent(const FString& TextContent);

	void ShowTeacherSpeak(bool bIsCorrect);

	void HideTeacherSpeak();
	void HideAnswerText();
};
