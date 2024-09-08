// Fill out your copyright notice in the Description page of Project Settings.


#include "KJH/Widget/KJH_WidgetBase.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Image.h"

void UKJH_WidgetBase::NativeConstruct()
{
    Super::NativeConstruct();

    ShowMouseCurser(true);
}

void UKJH_WidgetBase::NativeDestruct()
{
    Super::NativeDestruct();

    ShowMouseCurser(false);
}

void UKJH_WidgetBase::ShowMouseCurser(bool bValue)
{
    APlayerController* pc = GetWorld()->GetFirstPlayerController();
    if ( pc == nullptr) return;

    if ( bValue )
    {
        UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(pc, this);
        pc->SetShowMouseCursor(true);
    }
    else
    {
        UWidgetBlueprintLibrary::SetInputMode_GameOnly(pc);
        pc->SetShowMouseCursor(false);
    }
}
