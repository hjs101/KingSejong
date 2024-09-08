// Fill out your copyright notice in the Description page of Project Settings.


#include "KJH/KJH_PlayerController.h"
#include "KJH/Widget/KJH_OXQuizWidget.h"
#include "Net/UnrealNetwork.h"

void AKJH_PlayerController::ClientRPC_InitializeQuiz_Implementation(int32 QuizTime)
{
	if ( IsLocalPlayerController() )
	{
		QuizWidget = Cast<UKJH_OXQuizWidget>(CreateWidget(GetWorld(), QuizWidgetFactory));

		if ( QuizWidget )
		{
			QuizWidget->AddToViewport();
			QuizWidget->IdleState(QuizTime);

			UE_LOG(LogTemp, Warning, TEXT("Create QuizWidget!!"));

		}
	}
}

void AKJH_PlayerController::ClientRPC_ShowQuestionWidget_Implementation(const FString& Question)
{
	if ( IsValidQuizWidget() == false ) return;

	QuizWidget->QuestionState(Question);

	UE_LOG(LogTemp, Warning, TEXT("SetQuizInfo!!"));

}

void AKJH_PlayerController::ClientRPC_ShowWaitingWidget_Implementation()
{
	if ( IsValidQuizWidget() == false ) return;

	QuizWidget->WaitingState();

	UE_LOG(LogTemp, Warning, TEXT("ShowWaitingWidget!!"));

}

void AKJH_PlayerController::ClientRPC_ShowWidgetAnswer_Implementation(bool bCorrectAnswer, bool bSelectedAnswer)
{
	if ( IsValidQuizWidget() == false ) return;

	QuizWidget->AnswerState( bCorrectAnswer, bSelectedAnswer);

	// todo: 플레이어가 선택한 정답이랑 문제의 정답과 비교해서 점수처리 해야 함

	UE_LOG(LogTemp, Warning, TEXT("ShowWidgetAnswer!! : bCorrectAnswer(%d), bSelectedAnswer(%d)"), bCorrectAnswer, bSelectedAnswer);

}

void AKJH_PlayerController::ClientRPC_EndQuiz_Implementation()
{
	if ( IsValidQuizWidget() == false ) return;

	QuizWidget->EndState();

	UE_LOG(LogTemp, Warning, TEXT("EndQuiz!!"));

}

bool AKJH_PlayerController::IsValidQuizWidget()
{
	if(QuizWidget == nullptr)
		UE_LOG(LogTemp, Warning, TEXT("PlayerController의 QuizWidget이 없습니다."));

	return QuizWidget != nullptr;
}
