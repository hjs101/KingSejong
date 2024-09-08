// Fill out your copyright notice in the Description page of Project Settings.


#include "JJH/RunnerController.h"
#include "JJH/QuizWidget.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/CanvasPanel.h"

void ARunnerController::BeginPlay()
{
    Super::BeginPlay();

    //이걸 해야 복제됨
    bReplicates = true;
    // 서버에서 받은 데이터를 기반으로 위젯을 생성합니다.
}

void ARunnerController::ClientCreateQuizWidget_Implementation(const FWordsData& QuizData)
{
    if (IsLocalPlayerController()) // 로컬 플레이어 컨트롤러에서만 위젯 생성
    {
        if ( QuizWidgetInstance == nullptr )
        {
			if ( QuizWidgetClass )
			{
				QuizWidgetInstance = CreateWidget<UQuizWidget>(GetWorld() , QuizWidgetClass);
				if ( QuizWidgetInstance )
				{
					QuizWidgetInstance->AddToViewport();
					QuizWidgetInstance->InitializeQuiz(QuizData);
				}
			}
        }
        else
        {
            return;
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


void ARunnerController::ClientAbleInput_Implementation()
{
    SetInputMode(FInputModeGameOnly());
}

void ARunnerController::ClientShowLoading_Implementation()
{
    if ( IsLocalPlayerController() ) // 로컬 플레이어 컨트롤러에서만 위젯 생성
    {
        QuizWidgetInstance->ShowLoading();
    }
}

void ARunnerController::ClientSpectatePlayer_Implementation(const ARunnerController* TargetPlayer)
{
    if ( TargetPlayer )
    {
        // 타겟 플레이어의 뷰를 따라가기 위한 관전 모드 설정
        SetViewTargetWithBlend(TargetPlayer->GetPawn() , 1.0f);  // 타겟 플레이어의 뷰로 카메라 전환
        QuizWidgetInstance->HideLoading();
        GEngine->AddOnScreenDebugMessage(-1 , 5.f , FColor::Red , TEXT("1111"));
    }
}
