// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KJH_SpeechBubbleWidget.generated.h"

/**
 * 
 */
UCLASS()
class KINGSEJONG_API UKJH_SpeechBubbleWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

public:
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UTextBlock* TextMessage;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class USizeBox* SizeBox;

	UPROPERTY(EditDefaultsOnly)
	float PaddingValue = 100;


public:
	void SetTextMessage(FString Message);
};
