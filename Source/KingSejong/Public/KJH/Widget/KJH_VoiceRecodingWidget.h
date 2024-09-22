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
	FResponseVoiceChatbotResultSignature OnResponseQuestResultDelegate;

public:
	// UI
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	class UButton* BtnRecStart;
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	class UButton* BtnRecStop;
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	class UWidgetSwitcher* ButtonSwitcher;

	//UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	//class UButton* Btn_Question;


	// Temp
	//UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	//class UMultiLineEditableTextBox* EditTextBox_Question;

	FString RecordingFilePath = FString("");


private:
	UPROPERTY()
	class UKJH_VoiceRecorder* PlayerVoiceRecorder;

	UPROPERTY()
	class AKJH_HttpManager* HttpManager;

	bool bIsRecording = false;
	bool bIsRequest = false;

private:
	UFUNCTION(BlueprintCallable)
	void OnClickedBtnRecStart();
	UFUNCTION(BlueprintCallable)
	void OnClickedBtnRecStop();

};
