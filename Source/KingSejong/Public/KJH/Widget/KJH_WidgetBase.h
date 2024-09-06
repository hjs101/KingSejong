// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KJH_WidgetBase.generated.h"

UCLASS()
class KINGSEJONG_API UKJH_WidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:

	void ShowMouseCurser(bool bValue);

};
