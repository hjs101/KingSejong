// Fill out your copyright notice in the Description page of Project Settings.


#include "JJH/RunnerController.h"
#include "JJH/QuizWidget.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/CanvasPanel.h"
#include "JJH/RunningGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "JJH/EndGameWidget.h"
#include "JJH/JJH_GameInstance.h"
#include "JJH/JJHPlayerState.h"
#include "GameFramework/Character.h"
#include "JJH/Runner.h"

void ARunnerController::BeginPlay()
{
    Super::BeginPlay();

    //이걸 해야 복제됨
    bReplicates = true;

	UJJH_GameInstance* GI = Cast<UJJH_GameInstance>(GetWorld()->GetGameInstance());
	if (GI == nullptr)
	{
		return;
	}
    ARunner* Char = Cast<ARunner>(GetCharacter());
    if (Char)
    {
        UE_LOG(LogTemp, Warning, TEXT("!@#$!@#!@#!@21412"));
        ControllerMeshIndex = GI->CharacterMeshIndex;
        Char->GetMesh()->SetSkeletalMesh(GI->CharacterList[ControllerMeshIndex]);
    }
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
        //GEngine->AddOnScreenDebugMessage(-1 , 5.f , FColor::Red , TEXT("1111"));
    }
}


void ARunnerController::ClientHideAnswerText_Implementation()
{
    if ( IsLocalPlayerController() ) // 로컬 플레이어 컨트롤러에서만 위젯 생성
    {
        QuizWidgetInstance->HideAnswerText();
    }
}

void ARunnerController::MoveToNextPlayerWithDelay()
{
    FTimerHandle NextLevelTimerHandle;
    GetWorld()->GetTimerManager().SetTimer(NextLevelTimerHandle , this , &ARunnerController::MoveToNextPlayer , 2.0f , false);
}

void ARunnerController::MoveToNextPlayer()
{
    if ( HasAuthority() )  //서버
    {
        ARunningGameModeBase* GameMode = Cast<ARunningGameModeBase>(GetWorld()->GetAuthGameMode());
        if ( GameMode )
        {
            GameMode->MoveToNextPlayer();
        }
    }
    else //클라
    {
        ServerMoveToNextPlayer();
    }
}

void ARunnerController::ServerSubmitAnswer_Implementation(const FString& UserAnswer)
{
    ARunningGameModeBase* GameMode = Cast<ARunningGameModeBase>(GetWorld()->GetAuthGameMode());
    if ( GameMode )
    {
        GameMode->CheckAnswer(UserAnswer, this);
    }

}

void ARunnerController::ServerMoveToNextPlayer_Implementation()
{
    ARunningGameModeBase* GameMode = Cast<ARunningGameModeBase>(GetWorld()->GetAuthGameMode());
    if ( GameMode )
    {
        GameMode->MoveToNextPlayer();
    }
}

void ARunnerController::ClientShowTeacherSpeak_Implementation(bool bIsCorrect)
{
    if (!QuizWidgetInstance)
    {
        UE_LOG(LogTemp, Error, TEXT("QuizWidgetInstance is null in ClientShowTeacherSpeak"));
        return;
    }
    QuizWidgetInstance->ShowTeacherSpeak(bIsCorrect);
}

void ARunnerController::ClientShowInitials_Implementation()
{
    if ( QuizWidgetInstance )
    {
        QuizWidgetInstance->ShowInitials();  // UI에서 패널 표시
    }
}






//void ARunnerController::UpdateTextBoxContent(const FString& TextContent)
//{
//    위젯에서 컨트롤러 소환해서 컨트롤러에서 게임모드 불러서 서버 -> 멀티캐스트해달라해 
//    서버에서 입력자 위젯 찾아서 거기서 string찾아서 모두의 countdowntext에 입력시키기
//    ServerUpdateTextBoxContent(TextContent);
//}
//void ARunnerController::ServerUpdateTextBoxContent_Implementation(const FString& TextContent)
//{
//
//}
//void ARunnerController::MulticastUpdateTextBoxContent_Implementation(const FString& TextContent)
//{
//    QuizWidgetInstance->UpdateTextBoxContent(TextContent);
//}

void ARunnerController::SubmitAnswerTextToServer(const FString& TextContent)
{
    if ( HasAuthority() )
    {
        ARunningGameModeBase* GameMode = Cast<ARunningGameModeBase>(GetWorld()->GetAuthGameMode());
        if ( GameMode )
        {
            GameMode->UpdateTextInGameMode(TextContent);  // 서버에서 바로 처리);
        }
    }
    else
    {
        ServerSubmitAnswerTextToServer(TextContent);
    }
}

void ARunnerController::ServerSubmitAnswerTextToServer_Implementation(const FString& TextContent)
{
    ARunningGameModeBase* GameMode = Cast<ARunningGameModeBase>(GetWorld()->GetAuthGameMode());
    if ( GameMode )
    {
        GameMode->UpdateTextInGameMode(TextContent);  // 서버에서 바로 처리);
    }
}

void ARunnerController::ClientUpdateTextBoxContent_Implementation(const FString& TextContent)
{
    if ( QuizWidgetInstance )
    {
        QuizWidgetInstance->UpdateTextBoxContent(TextContent);  // 위젯의 텍스트박스 내용 업데이트
    }

}
void ARunnerController::ClientReturnToLobby_Implementation()
{
    // 로비 레벨로 이동
    UGameplayStatics::OpenLevel(this, FName("TestLobby"));
}

void ARunnerController::CreateEndGameWidget()
{
    UEndGameWidget* End = CreateWidget<UEndGameWidget>(GetWorld(), EndGameWidgetFactory);
    if (End)
    {			
        End->AddToViewport();
    }
}

void ARunnerController::ClientSwitchToEndWidget_Implementation()
{
    QuizWidgetInstance->SwitchToEnd();
}











