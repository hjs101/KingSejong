// Fill out your copyright notice in the Description page of Project Settings.

#include "JJH/QuizWidget.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Engine/Texture2D.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Styling/SlateBrush.h"
#include "UObject/ConstructorHelpers.h"
#include "Slate/SlateBrushAsset.h"



void UQuizWidget::InitializeQuiz(const FWordsData& WordData)
{
	CurrentWordData = WordData;
	
	//선생님 멘트
	TeacherSpeak->SetVisibility(ESlateVisibility::Visible);
	//퀴즈창 숨기기
	Quiz->SetVisibility(ESlateVisibility::Hidden);

	//3초후에 초성 보여주기 -> 후에 바꿔야함
	GetWorld()->GetTimerManager().SetTimer(ShowInitialTimerHandle, this, &UQuizWidget::ShowInitials, 3.f, false);

	//카운트다운 넘버 숨겨놓기
	CountDownText->SetText(FText::FromString(FString::FromInt(CountDownNum)));

	//웃는 훈장
	FString AssetPath = TEXT("/Game/JJH/UI/SmileTeacher");
	SmileTeacher = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass() , nullptr , *AssetPath));
	 
	//화난 훈장
	FString AssetPath2 = TEXT("/Game/JJH/UI/AngryTeacher");
	AngryTeacher = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass() , nullptr , *AssetPath2));

	PlayAnimation(TeacherIntro);
}

void UQuizWidget::ShowInitials()
{
	//로딩창 숨기기
	TeacherSpeak->SetVisibility(ESlateVisibility::Hidden);
	//퀴즈창 보여주기
	Quiz->SetVisibility(ESlateVisibility::Visible);
	//초성 보여주기
	Initials->SetText(FText::FromString(CurrentWordData.Initials));
	Initials->SetVisibility(ESlateVisibility::Visible);
	//뜻 보여주기
	Meaning->SetText(FText::FromString(CurrentWordData.Meaning));

	GetWorld()->GetTimerManager().ClearTimer(ShowInitialTimerHandle);
}

void UQuizWidget::ShowAnswerTextBox()
{
	QuizLoading->SetVisibility(ESlateVisibility::Hidden);
	
	AnswerHorizontal->SetVisibility(ESlateVisibility::Visible);
	Meaning->SetVisibility(ESlateVisibility::Visible);
	Initials->SetVisibility(ESlateVisibility::Visible);
}
void  UQuizWidget::HideLoading()
{
	QuizLoading->SetVisibility(ESlateVisibility::Hidden);
}
void UQuizWidget::ShowLoading()
{
	QuizLoading->SetVisibility(ESlateVisibility::Visible);
}
void UQuizWidget::StartCountDown()
{
	// 타이머 시작
	GetWorld()->GetTimerManager().SetTimer(CountDownTimerHandle, this, &UQuizWidget::UpdateCountDown, 1.0f, true);
	CountDownText->SetVisibility(ESlateVisibility::Visible);
}

void UQuizWidget::UpdateCountDown()
{
	//불값하나 만들어서 답 입력하면 사라지게끔해야함
	//카운트다운넘버 블루프린트에서 세팅해놓기
	if (CountDownNum > 0)
	{
		CountDownNum--;
		CountDownText->SetText(FText::FromString(FString::FromInt(CountDownNum)));
	}
	else
	{
		// 들어오면 종료텍스트 빼고 다 빼야함

		GetWorld()->GetTimerManager().ClearTimer(CountDownTimerHandle);

		QuizLoading->SetVisibility(ESlateVisibility::Hidden);
		CountDownText->SetText(FText::FromString(TEXT("종료")));
		Meaning->SetVisibility(ESlateVisibility::Hidden);
		Initials->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UQuizWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//정답칸 히든으로 숨겨놓기
	AnswerHorizontal->SetVisibility(ESlateVisibility::Hidden);
	//카운트다운
	CountDownText->SetVisibility(ESlateVisibility::Hidden);

	AnswerSubmitButton->OnClicked.AddDynamic(this, &UQuizWidget::SubmitAnswer);

	QuizLoading->SetVisibility(ESlateVisibility::Hidden);
}

void UQuizWidget::SubmitAnswer()
{
	if (AnswerTextBox )
	{
		FString UserAnswer = AnswerTextBox->GetText().ToString();
		FString Answer = CurrentWordData.Word;
		AnswerHorizontal->SetVisibility(ESlateVisibility::Hidden);
		if ( UserAnswer.Equals(Answer) )
		{
			//맞추면 퀴즈창 없애고 몇초 이따가 레벨 초기화
			Quiz->SetVisibility(ESlateVisibility::Hidden);
			TeacherSpeak->SetVisibility(ESlateVisibility::Visible);
			Teacher->SetBrushFromTexture(SmileTeacher);
			TeacherText->SetText(FText::FromString(TEXT("대단하구나!")));
		}
		else
		{
			//틀리면 퀴즈창 유지하고 다른 사람한테 넘어가기?
			TeacherSpeak->SetVisibility(ESlateVisibility::Visible);
			Teacher->SetBrushFromTexture(AngryTeacher);
			TeacherText->SetText(FText::FromString(TEXT("아니다 욘석아")));
			PlayAnimation(TeacherAngry, 0, 1, EUMGSequencePlayMode::PingPong);
		}
	}
}
