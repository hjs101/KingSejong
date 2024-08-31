// Fill out your copyright notice in the Description page of Project Settings.

#include "JJH/QuizWidget.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"

void UQuizWidget::InitializeQuiz(const FWordsData& WordData)
{
	CurrentWordData = WordData;
	Meaning->SetText(FText::FromString(WordData.Meaning));
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
