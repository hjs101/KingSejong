// Fill out your copyright notice in the Description page of Project Settings.


#include "KJH/Widget/KJH_OXQuizWidget.h"
#include "Components/Overlay.h"
#include "Components/EditableTextBox.h"
#include "Components/MultiLineEditableTextBox.h"
#include "Components/WidgetSwitcher.h"


void UKJH_OXQuizWidget::NativeConstruct()
{
    Super::NativeConstruct();

    SetActiveAnswerStatus(false);
}

void UKJH_OXQuizWidget::IdleState(int32 QuizTime)
{
    MaxQuizTime = QuizTime;
    CurrScore = 0;

    SetQuizWidgetState(EQuizWidgetState::Idle);
    ShowTimer(false);
}

void UKJH_OXQuizWidget::QuestionState(FString Quistion)
{
    SetActiveAnswerStatus(false);

    Text_Question->SetText(FText::FromString(Quistion));

    SetQuizWidgetState(EQuizWidgetState::Question);

    ShowTimer(true);
}

void UKJH_OXQuizWidget::WaitingState()
{
    ShowTimer(false);
    SetQuizWidgetState(EQuizWidgetState::Waiting);
}

void UKJH_OXQuizWidget::AnswerState(bool bCorrectAnswer, bool bSelectedAnswer)
{
    // 정답 표시
    EQuizWidgetState state = bCorrectAnswer ? EQuizWidgetState::AnswerTrue : EQuizWidgetState::AnswerFalse;
    SetQuizWidgetState(state);

    // 정답 여부
    bool bIsCorrect = bCorrectAnswer == bSelectedAnswer;
    SetActiveAnswerStatus(true, bIsCorrect);

    if ( bIsCorrect )
        CurrScore++;
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
    RemainingTime = MaxQuizTime;

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

        //SetQuizState(EQuizWidgetState::Waiting);
    }
}

void UKJH_OXQuizWidget::SetQuizWidgetState(EQuizWidgetState State)
{
    WidgetSwitcher_Quiz->SetActiveWidgetIndex((int32)State);
}

void UKJH_OXQuizWidget::SetActiveAnswerStatus(bool bActive, bool bIsCorrect)
{
    if ( bActive )
    {
        Overlay_Answer->SetVisibility(ESlateVisibility::Visible);

        int32 index = bIsCorrect ? 0 : 1;
        WidgetSwitcher_Answer->SetActiveWidgetIndex(index);
    }
    else
    {
        Overlay_Answer->SetVisibility(ESlateVisibility::Hidden);

    }
}
