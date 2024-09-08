// Fill out your copyright notice in the Description page of Project Settings.


#include "KJH/Widget/KJH_OXQuizWidget.h"
#include "Components/Overlay.h"
#include "Components/EditableTextBox.h"
#include "Components/MultiLineEditableTextBox.h"
#include "Components/WidgetSwitcher.h"


void UKJH_OXQuizWidget::NativeConstruct()
{
    Super::NativeConstruct();


    SetQuizState(EQuizWidgetState::Idle);
}

void UKJH_OXQuizWidget::SetQuizState(EQuizWidgetState State)
{
    QuizState = State;

    switch ( QuizState )
    {
        case EQuizWidgetState::Idle:
            IdleState();
            break;
        case EQuizWidgetState::Question:
            QuestionState();
            break;
        case EQuizWidgetState::Waiting:
            WaitingState();
            break;
        case EQuizWidgetState::Answer:
            AnswerState();
            break;
        case EQuizWidgetState::End:
            EndState();
            break;
        default:
            break;
    }


    UE_LOG(LogTemp, Warning, TEXT("%s"), *UEnum::GetValueAsString(QuizState));

    if ( QuizState == EQuizWidgetState::Answer || QuizState == EQuizWidgetState::End ) return;

    WidgetSwitcher_QA->SetActiveWidgetIndex(( int32 ) QuizState);
}

void UKJH_OXQuizWidget::IdleState()
{
    ShowTimer(false);

    FTimerHandle timerHandle;
    GetWorld()->GetTimerManager().SetTimer(timerHandle, FTimerDelegate::CreateLambda([ this ] ()
        {
            SetQuizState(EQuizWidgetState::Question);
        }),
        IdleTime, false);
}

void UKJH_OXQuizWidget::QuestionState()
{
    SetQuizAndAnswer(TEXT("인어공주는 6번째 공주이다"), true);

    ShowTimer(true);

}

void UKJH_OXQuizWidget::WaitingState()
{
    ShowTimer(false);

    // 정답 공개
    FTimerHandle timerHandle;
    GetWorld()->GetTimerManager().SetTimer(timerHandle, FTimerDelegate::CreateLambda([ this ] ()
        {
            SetQuizState(EQuizWidgetState::Answer);

        }),
        WaitingTime, false);
}

void UKJH_OXQuizWidget::AnswerState()
{
    // 정답에 따라 OX 이미지 보여주기
    int32 index = ( int32 ) QuizState;
    if ( bCurrAnswer == false )
        index++;

    WidgetSwitcher_QA->SetActiveWidgetIndex(index);

    // 퀴즈 카운드 증가
    QuizCount++;

    FTimerHandle timerHandle;
    GetWorld()->GetTimerManager().SetTimer(timerHandle, FTimerDelegate::CreateLambda([ this ] ()
        {
            // 다음 퀴즈
            if ( QuizCount < MaxQuizCount )
            {
                SetQuizState(EQuizWidgetState::Question);
            }
            // 퀴즈 종료
            else
            {
                SetQuizState(EQuizWidgetState::End);
            }
        }),
        AnwserTime, false);
}

void UKJH_OXQuizWidget::GetQuiz()
{
}


void UKJH_OXQuizWidget::SetQuizAndAnswer(FString Quistion, bool bAnswer)
{
    Text_Quiz->SetText(FText::FromString(Quistion));

    bCurrAnswer = bAnswer;
}

void UKJH_OXQuizWidget::ShowTimer(bool bValue)
{
    if ( bValue )
    {
        Overlay_Timer->SetVisibility(ESlateVisibility::Visible);
        StartTimer();
    }
    else
    {
        Overlay_Timer->SetVisibility(ESlateVisibility::Hidden);
    }

}

void UKJH_OXQuizWidget::StartTimer()
{
    // 남은 시간 초기화
    RemainingTime = QuizTime;

    // 타이머 텍스트 초기화
    Text_Timer->SetText(FText::AsNumber(RemainingTime));

    // 1초마다 UpdateTimer 함수 호출
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UKJH_OXQuizWidget::UpdateTimer, 1.0f, true);
}

void UKJH_OXQuizWidget::UpdateTimer()
{
    // 타이머 실행 중
    if ( RemainingTime > 0 )
    {
        RemainingTime--;
        Text_Timer->SetText(FText::AsNumber(RemainingTime));
    }
    // 타이머가 다 되었을 때
    else
    {
        Text_Timer->SetText(FText::AsNumber(0));
        GetWorld()->GetTimerManager().ClearTimer(TimerHandle);

        SetQuizState(EQuizWidgetState::Waiting);

    }
}