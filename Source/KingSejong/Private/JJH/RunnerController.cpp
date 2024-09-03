﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "JJH/RunnerController.h"
#include "JJH/QuizWidget.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"


void ARunnerController::BeginPlay()
{
    Super::BeginPlay();

    // 서버에서 받은 데이터를 기반으로 위젯을 생성합니다.
}

void ARunnerController::ClientCreateQuizWidget_Implementation(const FWordsData& QuizData)
{
    if (IsLocalPlayerController()) // 로컬 플레이어 컨트롤러에서만 위젯 생성
    {
        if (QuizWidgetClass)
        {
			QuizWidgetInstance = CreateWidget<UQuizWidget>(GetWorld(), QuizWidgetClass);
			if (QuizWidgetInstance)
			{               
				QuizWidgetInstance->AddToViewport();
				QuizWidgetInstance->InitializeQuiz(QuizData);
			}
        }

    }
}
void ARunnerController::ClientShowAnswerTextBox_Implementation()
{
    if (IsLocalPlayerController()) // 로컬 플레이어 컨트롤러에서만 위젯 생성
    {
        QuizWidgetInstance->ShowAnswerTextBox();
    }
}

void ARunnerController::ClientStartWidgetCountDown_Implementation()
{
    if (IsLocalPlayerController()) // 로컬 플레이어 컨트롤러에서만 위젯 생성
    {
        QuizWidgetInstance->StartCountDown();
    }
}

void ARunnerController::ClientDisableInput_Implementation()
{
    SetInputMode(FInputModeUIOnly());
}
