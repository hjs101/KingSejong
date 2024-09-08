// Fill out your copyright notice in the Description page of Project Settings.


#include "KJH/KJH_PlayerController.h"

void AKJH_PlayerController::SetQuizWidgetState(EQuizState State)
{
	UE_LOG(LogTemp, Warning, TEXT("SetQuizWidgetState!!"));

}

void AKJH_PlayerController::SetQuizInfo()
{
	UE_LOG(LogTemp, Warning, TEXT("SetQuizInfo!!"));

}

void AKJH_PlayerController::ShowWidgetWaiting()
{
	UE_LOG(LogTemp, Warning, TEXT("ShowWidgetWaiting!!"));

}

void AKJH_PlayerController::ShowWidgetAnswer(bool bCorrectAnswer, bool bSelectedAnswer)
{
	UE_LOG(LogTemp, Warning, TEXT("ShowWidgetAnswer!! : bCorrectAnswer(%d), bSelectedAnswer(%d)"), bCorrectAnswer, bSelectedAnswer);

}

void AKJH_PlayerController::EndQuiz()
{
	UE_LOG(LogTemp, Warning, TEXT("EndQuiz!!"));

}
