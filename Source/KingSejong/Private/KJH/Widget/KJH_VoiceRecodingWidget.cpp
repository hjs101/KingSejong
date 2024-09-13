// Fill out your copyright notice in the Description page of Project Settings.


#include "KJH/Widget/KJH_VoiceRecodingWidget.h"
#include "KJH/KJH_Player.h"
#include "KJH/KJH_VoiceRecorder.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"
#include "KJH/API/KJH_HttpManager.h"
#include "Components/EditableTextBox.h"
#include "Components/MultiLineEditableTextBox.h"

void UKJH_VoiceRecodingWidget::NativeConstruct()
{
    Super::NativeConstruct();

    auto* player = Cast<AKJH_Player>(GetWorld()->GetFirstPlayerController()->GetPawn());

    if ( player )
    {
        PlayerVoiceRecorder = player->GetComponentByClass<UKJH_VoiceRecorder>();

        if ( PlayerVoiceRecorder )
        {
            BtnRecStart->OnClicked.AddDynamic(this, &UKJH_VoiceRecodingWidget::OnClickedBtnRecStart);
            BtnRecStop->OnClicked.AddDynamic(this, &UKJH_VoiceRecodingWidget::OnClickedBtnRecStop);


            //RecordingFilePath = fpath


            UE_LOG(LogTemp, Warning, TEXT("VoiceRecorder 를 바인딩!!!"));
        }

    }

    //// HttpManager
    //HttpManager = Cast<AKJH_HttpManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AKJH_HttpManager::StaticClass()));



    //Btn_Question->OnClicked.AddDynamic(this, &UKJH_VoiceRecodingWidget::Req_Question);
    
}

void UKJH_VoiceRecodingWidget::NativeDestruct()
{
    Super::NativeDestruct();

    if( OnCloseWidgetDelegate.IsBound()&& bIsRequest == false)
        OnCloseWidgetDelegate.Broadcast();

    if( bIsRecording && PlayerVoiceRecorder )
        PlayerVoiceRecorder->OnStopRecord();
}

void UKJH_VoiceRecodingWidget::OnClickedBtnRecStart()
{
    if(PlayerVoiceRecorder == nullptr) return;
    
    // 녹음 시작
    bool bResult = PlayerVoiceRecorder->OnStartRecord();
    if( bResult )
    {
        bIsRecording = true;

        ButtonSwitcher->SetActiveWidgetIndex(1);
    }
}

void UKJH_VoiceRecodingWidget::OnClickedBtnRecStop()
{
    if ( PlayerVoiceRecorder == nullptr ) return;

    // 녹음 종료 및 저장
    bool bResult = PlayerVoiceRecorder->OnStopRecord();

    if ( bResult )
    {
        bIsRecording = false;
        bIsRequest = true;
        
        PlayerVoiceRecorder->SendToChatbot();
        // Req_Question();


        RemoveFromParent();
    }
}

//void UKJH_VoiceRecodingWidget::Req_Question()
//{
//    // 질문 API 호출
//    if ( HttpManager )
//    {
//        // 텍스트 소통
//        //FText text = EditTextBox_Question->GetText();
//        //HttpManager->Req_BookAnswer(TEXT(""), text.ToString());
//
//        // 파일 전송
//        HttpManager->Req_AskByFileToChatbot(TEXT(""), TEXT(""));
//
//        bIsRequest = true;
//    }
//
//    RemoveFromParent();
//}
