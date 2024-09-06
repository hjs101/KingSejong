// Fill out your copyright notice in the Description page of Project Settings.


#include "JJH/RunnerController.h"
#include "JJH/QuizWidget.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/CanvasPanel.h"
#include "JJH/RunningGameModeBase.h"

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

void ARunnerController::ClientHideLoading_Implementation()
{
    if ( IsLocalPlayerController() ) // 로컬 플레이어 컨트롤러에서만 위젯 생성
    {
        QuizWidgetInstance->HideLoading();
    }

}
void ARunnerController::ClientSpectatePlayer_Implementation(AActor* TargetPlayer)
{
    if ( TargetPlayer )
    {
        // 타겟 플레이어의 뷰를 따라가기 위한 관전 모드 설정
        SetViewTargetWithBlend(TargetPlayer , 1.0f);  // 타겟 플레이어의 뷰로 카메라 전환
        if ( IsLocalPlayerController() ) // 로컬 플레이어 컨트롤러에서만 위젯 생성
        {
            QuizWidgetInstance->HideLoading();
        }
        GEngine->AddOnScreenDebugMessage(-1 , 5.f , FColor::Red , TEXT("1111"));
    }
}

void ARunnerController::ServerUpdateTextBoxContent_Implementation(const FString& TextContent)
{
    MulticastUpdateTextBoxContent(TextContent);
}

void ARunnerController::MulticastUpdateTextBoxContent_Implementation(const FString& TextContent)
{
    QuizWidgetInstance->UpdateTextBoxContent(TextContent);

}

void ARunnerController::ServerMoveToNextPlayer_Implementation()
{
    ARunningGameModeBase* GameMode = Cast<ARunningGameModeBase>(GetWorld()->GetAuthGameMode());
    if ( GameMode )
    {
        GameMode->MoveToNextPlayer();
    }
}


void ARunnerController::MoveToNextPlayerWithDelay()
{
    FTimerHandle NextLevelTimerHandle;
    GetWorld()->GetTimerManager().SetTimer(NextLevelTimerHandle , this , &ARunnerController::MoveToNextPlayer , 2.0f , false);
}

void ARunnerController::MoveToNextPlayer()
{
    if ( !HasAuthority() )  // 클라이언트에서 실행
    {
        ServerMoveToNextPlayer();
    }
}

