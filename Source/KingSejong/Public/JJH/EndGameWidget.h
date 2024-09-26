// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EndGameWidget.generated.h"

/**
 * 
 */
UCLASS()
class KINGSEJONG_API UEndGameWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

	void OnRestartButtonClicked();
	void OnQuitButtonClicked();
	UPROPERTY(meta = (BindWidget))
	class UButton* QuitButton;		
	
	UPROPERTY(meta = (BindWidget))
	class UButton* RestartButton;
};
