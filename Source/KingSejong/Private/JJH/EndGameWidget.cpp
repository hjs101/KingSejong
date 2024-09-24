// Fill out your copyright notice in the Description page of Project Settings.


#include "JJH/EndGameWidget.h"
#include "Components/Button.h"
#include "JJH/JJH_GameInstance.h"

void UEndGameWidget::NativeConstruct()
{
	QuitButton->OnClicked.AddDynamic(this, &UEndGameWidget::OnQuitButtonClicked);
	RestartButton->OnClicked.AddDynamic(this, &UEndGameWidget::OnRestartButtonClicked);
}

void UEndGameWidget::OnRestartButtonClicked()
{
	
}
void UEndGameWidget::OnQuitButtonClicked()
{
	UJJH_GameInstance* gi = Cast<UJJH_GameInstance>(GetWorld()->GetGameInstance());
	if (gi)
	{
		gi->ExitSession();
	}
}