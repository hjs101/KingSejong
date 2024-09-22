// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BattleResultWidget.generated.h"

/**
 * 
 */
UCLASS()
class KINGSEJONG_API UBattleResultWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:
	virtual void NativeConstruct() override;

	UPROPERTY()
	class AHJS_BattlePlayer* Me;

	UPROPERTY(meta=(BindWidget))
	class UTextBlock* ResultText;

	UPROPERTY(meta=(BindWidget))
	class UButton* RestartBtn;

	UPROPERTY(meta=(BindWidget))
	class UButton* ExitBtn;

	UPROPERTY(BlueprintReadWrite)
	FString EndText;

	void WinnerSetting();

	void LoserSetting();

	UFUNCTION()
	void RequestRestart();

	UFUNCTION()
	void ExitToLobby();

	void SetEndText(FString NewText);


};
