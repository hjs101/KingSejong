// Fill out your copyright notice in the Description page of Project Settings.


#include "KJH/Widget/KJH_VoiceRecodingWidget.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "KJH/KJH_Player.h"
#include "KJH/KJH_VoiceRecorder.h"

void UKJH_VoiceRecodingWidget::NativeConstruct()
{
    Super::NativeConstruct();

    auto* player = Cast<AKJH_Player>(GetWorld()->GetFirstPlayerController()->GetPawn());

    if ( player )
    {
        VoiceRecorder = player->GetComponentByClass<UKJH_VoiceRecorder>();

        if ( VoiceRecorder )
        {
            BtnRecStart->OnClicked.AddDynamic(this, &UKJH_VoiceRecodingWidget::OnClickedBtnRecStart);
            BtnRecStop->OnClicked.AddDynamic(this, &UKJH_VoiceRecodingWidget::OnClickedBtnRecStop);

            UE_LOG(LogTemp, Warning, TEXT("VoiceRecorder 를 바인딩!!!"));
        }
    }
}

void UKJH_VoiceRecodingWidget::NativeDestruct()
{
    Super::NativeDestruct();

    if( OnCloseWidgetDelegate.IsBound())
        OnCloseWidgetDelegate.Broadcast();

    if( bIsRecording && VoiceRecorder )
        VoiceRecorder->OnStopRecord();
}

void UKJH_VoiceRecodingWidget::OnClickedBtnRecStart()
{
    if(VoiceRecorder == nullptr) return;
    
    bool bResult = VoiceRecorder->OnStartRecord();

    if( bResult )
    {
        bIsRecording = true;

        ButtonSwitcher->SetActiveWidgetIndex(1);
    }
}

void UKJH_VoiceRecodingWidget::OnClickedBtnRecStop()
{
    if ( VoiceRecorder == nullptr ) return;
    
    bool bResult = VoiceRecorder->OnStopRecord();

    // todo : 통신 추가
    if ( bResult )
    {
        bIsRecording = false;

        ButtonSwitcher->SetActiveWidgetIndex(0);

        if ( OnSuccessedAnswerDelegate.IsBound())
            OnSuccessedAnswerDelegate.Broadcast(FString("Say~~~~~"));
    }
}
