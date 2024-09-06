// Fill out your copyright notice in the Description page of Project Settings.


#include "KJH/Widget/KJH_SpeechBubbleWidget.h"
#include "Components/TextBlock.h"
#include "Components/SizeBox.h"

void UKJH_SpeechBubbleWidget::NativeConstruct()
{
    Super::NativeConstruct();
}

void UKJH_SpeechBubbleWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

}

void UKJH_SpeechBubbleWidget::SetTextMessage(FString Message)
{
    TextMessage->SetText(FText::FromString(Message));

}
