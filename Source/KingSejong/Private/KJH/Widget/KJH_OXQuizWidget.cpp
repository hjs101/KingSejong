// Fill out your copyright notice in the Description page of Project Settings.


#include "KJH/Widget/KJH_OXQuizWidget.h"
#include "Components/Overlay.h"
#include "Components/EditableTextBox.h"
#include "Components/MultiLineEditableTextBox.h"
#include "Components/WidgetSwitcher.h"

/*
퀴즈 시간이 시작되면 UI가 나타난다.

퀴즈 문제를 가져온다.


퀴즈를 설정한다.
타이머가 실행된다.

타이머 시간이 지나면 정답이 공개된다.

정답이 공개되고 일정 시간 이후 다음 퀴즈 문제가 설정된다.

모든 문제가 제출되고 나면 타이머가 종료되고 UI를 닫는다.


*/


void UKJH_OXQuizWidget::NativeConstruct()
{
    Super::NativeConstruct();

    SetQuizState(EQuizState::Idle);
}

void UKJH_OXQuizWidget::SetQuizState(EQuizState State)
{
    QuizState = State;

    switch ( QuizState )
    {
        case EQuizState::Idle:
            IdleState();
            break;
        case EQuizState::Question:
            QuestionState();
            break;
        case EQuizState::Waiting:
            WaitingState();
            break;
        case EQuizState::Answer:
            AnswerState();
            break;
        case EQuizState::End:
            EndState();
            break;
        default:
            break;
    }


    UE_LOG(LogTemp, Warning, TEXT("%s"), *UEnum::GetValueAsString(QuizState));

    if ( QuizState == EQuizState::Answer || QuizState == EQuizState::End ) return;

    WidgetSwitcher_QA->SetActiveWidgetIndex(( int32 ) QuizState);
}

void UKJH_OXQuizWidget::IdleState()
{
    ShowTimer(false);

    FTimerHandle timerHandle;
    GetWorld()->GetTimerManager().SetTimer(timerHandle, FTimerDelegate::CreateLambda([ this ] ()
        {
            SetQuizState(EQuizState::Question);
        }),
        IdleTime, false);
}

void UKJH_OXQuizWidget::QuestionState()
{
    SetQuizAndAnswer(TEXT("정답은 true일것이야!"), true);

    ShowTimer(true);

}

void UKJH_OXQuizWidget::WaitingState()
{
    ShowTimer(false);

    // 정답 공개
    FTimerHandle timerHandle;
    GetWorld()->GetTimerManager().SetTimer(timerHandle, FTimerDelegate::CreateLambda([ this ] ()
        {
            SetQuizState(EQuizState::Answer);

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
                SetQuizState(EQuizState::Question);
            }
            // 퀴즈 종료
            else
            {
                SetQuizState(EQuizState::End);
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

        SetQuizState(EQuizState::Waiting);

    }
}