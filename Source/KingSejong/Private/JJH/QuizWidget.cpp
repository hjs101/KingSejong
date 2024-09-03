// Fill out your copyright notice in the Description page of Project Settings.

#include "JJH/QuizWidget.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Engine/Texture2D.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Styling/SlateBrush.h"
#include "UObject/ConstructorHelpers.h"
#include "Slate/SlateBrushAsset.h"



void UQuizWidget::InitializeQuiz(const FWordsData& WordData)
{
	CurrentWordData = WordData;
	
	//로딩창 보여주기
	Loading->SetVisibility(ESlateVisibility::Visible);
	//퀴즈창 숨기기
	Quiz->SetVisibility(ESlateVisibility::Hidden);

	//3초후에 초성 보여주기 -> 후에 바꿔야함
	GetWorld()->GetTimerManager().SetTimer(ShowInitialTimerHandle, this, &UQuizWidget::ShowInitials, 3.f, false);

	//카운트다운 넘버 숨겨놓기
	CountDownText->SetText(FText::FromString(FString::FromInt(CountDownNum)));

	FString AssetPath = TEXT("/Game/JJH/UI/SmileTeacher");
	SmileTeacher = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass() , nullptr , *AssetPath));

	//static ConstructorHelpers::FObjectFinder<UTexture> TextureFinder(TEXT("/Game/JJH/UI/SmileTeacher"));
	//if ( TextureFinder.Succeeded() )
	//{
	//	SmileTeacher = TextureFinder.Object;
	//}
	//else
	//{
	//	UKismetSystemLibrary::QuitGame(GetWorld() , nullptr , EQuitPreference::Quit , false);
	//}
}

void UQuizWidget::ShowInitials()
{
	//로딩창 숨기기
	Loading->SetVisibility(ESlateVisibility::Hidden);
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
	AnswerTextBox->SetVisibility(ESlateVisibility::Visible);
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
		// 타이머 종료
		GetWorld()->GetTimerManager().ClearTimer(CountDownTimerHandle);
		CountDownText->SetText(FText::FromString(TEXT("Finish!")));
		
	}
}

void UQuizWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//정답칸 히든으로 숨겨놓기
	AnswerTextBox->SetVisibility(ESlateVisibility::Hidden);
	//카운트다운
	CountDownText->SetVisibility(ESlateVisibility::Hidden);

	AnswerSubmitButton->OnClicked.AddDynamic(this, &UQuizWidget::SubmitAnswer);


}

void UQuizWidget::SubmitAnswer()
{
	if (AnswerTextBox )
	{
		FString UserAnswer = AnswerTextBox->GetText().ToString();
		FString Answer = CurrentWordData.Word;

		if ( UserAnswer.Equals(Answer) )
		{
			Loading->SetVisibility(ESlateVisibility::Visible);
			Teacher->SetBrushFromTexture(SmileTeacher);
			TeacherText->SetText(FText::FromString(TEXT("대단하구나!")));
		}
		else
		{
			Loading->SetVisibility(ESlateVisibility::Visible);
			TeacherText->SetText(FText::FromString(TEXT("아니다 이 녀석아")));
		}
	}
}
