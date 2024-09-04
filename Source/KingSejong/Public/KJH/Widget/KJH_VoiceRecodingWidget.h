// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KJH/Widget/KJH_WidgetBase.h"
#include "KJH_VoiceRecodingWidget.generated.h"

DECLARE_MULTICAST_DELEGATE(FCloseWidgetSignature);
DECLARE_MULTICAST_DELEGATE_OneParam(FSuccessedAnswerSignature, FString);
DECLARE_MULTICAST_DELEGATE_TwoParams(FResponseVoiceChatbotResultSignature, bool, FString);

UCLASS()
class KINGSEJONG_API UKJH_VoiceRecodingWidget : public UKJH_WidgetBase
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	FCloseWidgetSignature OnCloseWidgetDelegate;
	FResponseVoiceChatbotResultSignature OnResponseVoiceChatbotResultDelegate;

public:
	// UI
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	class UButton* BtnRecStart;
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	class UButton* BtnRecStop;
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	class UWidgetSwitcher* ButtonSwitcher;

private:
	UPROPERTY()
	class UKJH_VoiceRecorder* VoiceRecorder;

	bool bIsRecording = false;
	bool bResponseResult = false;

private:
	UFUNCTION(BlueprintCallable)
	void OnClickedBtnRecStart();
	UFUNCTION(BlueprintCallable)
	void OnClickedBtnRecStop();

};
