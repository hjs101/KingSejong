// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BattleQuestionStruct.h"
#include "MainUI.generated.h"

/**
 * 
 */
UCLASS()
class KINGSEJONG_API UMainUI : public UUserWidget
{
	GENERATED_BODY()
	

public:

	virtual void NativeConstruct() override;
	
	class AHJS_BattlePlayer* Me;

	UPROPERTY(meta=(BindWidget))
	class UImage* Player1_HP1;
	UPROPERTY(meta=(BindWidget))
	class UImage* Player1_HP2;
	UPROPERTY(meta=(BindWidget))
	class UImage* Player1_HP3;

	UPROPERTY(meta=(BindWidget))
	class UImage* Player2_HP1;
	UPROPERTY(meta=(BindWidget))
	class UImage* Player2_HP2;
	UPROPERTY(meta=(BindWidget))
	class UImage* Player2_HP3;

	UPROPERTY(meta=(BindWidget))
	class UTextBlock* Speech;

	UPROPERTY(meta=(BindWidget))
	class UImage* Teacher;

	UPROPERTY(meta=(BindWidget))
	class UBorder* OptionCanvas;

	UPROPERTY(meta=(BindWidget))
	class UHorizontalBox* Options;

	UPROPERTY(meta=(BindWidget))
	class UTextBlock* AnswerOption1;
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* AnswerOption2;
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* AnswerOption3;
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* AnswerOption4;

	UPROPERTY(meta=(BindWidget))
	class UTextBlock* CountDown;

	UPROPERTY(BlueprintReadWrite)
	FString LineText;

	UPROPERTY(BlueprintReadWrite)
	FString CountDownText;

	

	UPROPERTY(BlueprintReadWrite)
	FString Answer1;
	UPROPERTY(BlueprintReadWrite)
	FString Answer2;
	UPROPERTY(BlueprintReadWrite)
	FString Answer3;
	UPROPERTY(BlueprintReadWrite)
	FString Answer4;

	void SetHP(int32 PlayerNum,int32 CurrentHP);

	void GameStartUIInit();

	void TurnStartUIInit();

	FTimerHandle CountDownTimerHandle;

	void OnCountDown();

	void RecordStartUIInit(FBattleQuestionData Data);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UTexture2D* Smile;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UTexture2D* Angry;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UTexture2D* Idle;

	void SetTeacherLine(FString Text, int32 TeacherMode);

	void SetOptions(TArray<FString> OptionsArr);

	TArray<FString> TutorialTextArr;

	FString FullText;
	FString CurrentText;

	UPROPERTY(EditAnywhere)
	float TextSpeed = 0.1f;
	float NextTextStartRate = 1.f;
	FTimerHandle TextAnimTimerHandle;
	FTimerHandle NextTextTimerHandle;
	int32 CurrentNum = -1;
	void SetTextToDisplay(const FString& NewText);

	UFUNCTION()
	void UpdateText();

	UFUNCTION()
	void StartTutorialUI(int32 Num);

	UFUNCTION()
	void NextTextSet();
}; 
