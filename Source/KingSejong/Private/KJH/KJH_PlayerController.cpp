// Fill out your copyright notice in the Description page of Project Settings.


#include "KJH/KJH_PlayerController.h"
#include "KJH/Widget/KJH_OXQuizWidget.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "KJH/KJH_QuizSoundHandler.h"

AKJH_PlayerController::AKJH_PlayerController()
{
	AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComp"));
	AudioComp->SetupAttachment(RootComponent);

	SoundHandler = CreateDefaultSubobject<UKJH_QuizSoundHandler>(TEXT("SoundHandler"));
}


void AKJH_PlayerController::ClientRPC_InitializeQuiz_Implementation(int32 QuizTime)
{
	if ( IsLocalPlayerController() )
	{
		QuizWidget = Cast<UKJH_OXQuizWidget>(CreateWidget(GetWorld(), QuizWidgetFactory));

		if ( QuizWidget )
		{
			QuizWidget->AddToViewport();
			QuizWidget->IdleState(QuizTime);

			SoundHandler->PlayQuizSound(SFX_Bright);
			//UGameplayStatics::PlaySound2D(GetWorld(), SFX_Bright);

			UE_LOG(LogTemp, Warning, TEXT("Create QuizWidget!!"));
		}
	}
}

void AKJH_PlayerController::ClientRPC_ShowQuestionWidget_Implementation(const FString& Question)
{
	if ( IsValidQuizWidget() == false ) return;

	QuizWidget->QuestionState(Question);

	//UGameplayStatics::PlaySound2D(GetWorld(), SFX_Tick);
	SoundHandler->PlayQuizSound(SFX_Tick);

	UE_LOG(LogTemp, Warning, TEXT("SetQuizInfo!!"));

}

void AKJH_PlayerController::ClientRPC_ShowWaitingWidget_Implementation()
{
	if ( IsValidQuizWidget() == false ) return;

	QuizWidget->WaitingState();
	//UGameplayStatics::PlaySound2D(GetWorld(), SFX_Buzzer);
	SoundHandler->PlayQuizSound(SFX_Buzzer);

	UE_LOG(LogTemp, Warning, TEXT("ShowWaitingWidget!!"));

}

void AKJH_PlayerController::ClientRPC_ShowWidgetAnswer_Implementation(bool bCorrectAnswer, bool bSelectedAnswer)
{
	if ( IsValidQuizWidget() == false ) return;

	QuizWidget->AnswerState( bCorrectAnswer, bSelectedAnswer);

	// todo: 플레이어가 선택한 정답이랑 문제의 정답과 비교해서 점수처리 해야 함

	// 정답 여부에 따른 사운드 및 카메라 쉐이킹
    bool bIsCorrect = bCorrectAnswer == bSelectedAnswer;
	if (bIsCorrect)
	{
		SoundHandler->PlayQuizSound(SFX_Correct);
	}
	else
	{
		SoundHandler->PlayQuizSound(SFX_Fail);
		ClientStartCameraShake(CameraShake);
	}

    UE_LOG(LogTemp, Warning, TEXT("ShowWidgetAnswer!! : bCorrectAnswer(%d), bSelectedAnswer(%d)"), bCorrectAnswer, bSelectedAnswer);
}

void AKJH_PlayerController::ClientRPC_EndQuiz_Implementation()
{
	if ( IsValidQuizWidget() == false ) return;

	QuizWidget->EndState();

	//UGameplayStatics::PlaySound2D(GetWorld(), SFX_Bright);

	SoundHandler->PlayQuizSound(SFX_Bright);


	UE_LOG(LogTemp, Warning, TEXT("EndQuiz!!"));

}

bool AKJH_PlayerController::IsValidQuizWidget()
{
	if(QuizWidget == nullptr)
		UE_LOG(LogTemp, Warning, TEXT("PlayerController의 QuizWidget이 없습니다."));

	return QuizWidget != nullptr;
}

void AKJH_PlayerController::PlayQuizSound(USoundBase* Sound)
{
	if (AudioComp)
	{
		//OwnerPC->ClientPlaySound(Sound);
		AudioComp->Stop();

		AudioComp->Sound = Sound;
		AudioComp->Play();
	}
}
