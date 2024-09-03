// Fill out your copyright notice in the Description page of Project Settings.


#include "KJH/Widget/KJH_SpeechBubbleWidget.h"
#include "Components/TextBlock.h"

void UKJH_SpeechBubbleWidget::NativeConstruct()
{
    Super::NativeConstruct();

    //SetTextMessage(FString("OOOOOOOOOOOOOOOOOOOOOOOOOOO"));
    TextMessage->SetAutoWrapText(true);
    TextMessage->SetWrapTextAt(1000);

}

void UKJH_SpeechBubbleWidget::SetTextMessage(FString Message)
{
    TextMessage->SetText(FText::FromString(Message));
}
