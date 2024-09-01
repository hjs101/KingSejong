// Fill out your copyright notice in the Description page of Project Settings.

#include "JJH/QuizWidget.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"

void UQuizWidget::InitializeQuiz(const FWordsData& WordData)
{
	CurrentWordData = WordData;
	Meaning->SetText(FText::FromString(WordData.Meaning));
	//정답칸 히든으로 숨겨놓기
	AnswerTextBox->SetVisibility(ESlateVisibility::Hidden);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, WordData.Meaning);
	GetWorld()->GetTimerManager().SetTimer(ShowInitialTimerHandle, this, &UQuizWidget::ShowInitials, 3.f, false);
}

void UQuizWidget::ShowInitials()
{
	Initials->SetText(FText::FromString(CurrentWordData.Initials));
	Initials->SetVisibility(ESlateVisibility::Visible);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("SetText2"));
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
}

void UQuizWidget::UpdateCountDown()
{
	//불값하나 만들어서 답 입력하면 사라지게끔해야함
	if (CountDownNum > 0)
	{
		CountDownNum--;
		CountDownText->SetText(FText::FromString(FString::FromInt(CountDownNum)));
	}
	else
	{
		// 타이머 종료
		GetWorld()->GetTimerManager().ClearTimer(CountDownTimerHandle);
		
	}
}
