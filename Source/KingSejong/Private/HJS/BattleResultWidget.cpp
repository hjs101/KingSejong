// Fill out your copyright notice in the Description page of Project Settings.


#include "HJS/BattleResultWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "HJS/HJS_BattlePlayer.h"
void UBattleResultWidget::NativeConstruct()
{
	Super::NativeConstruct();
	RestartBtn->OnClicked.AddDynamic(this, &UBattleResultWidget::RequestRestart);
	ExitBtn->OnClicked.AddDynamic(this, &UBattleResultWidget::ExitToLobby);
}

void UBattleResultWidget::WinnerSetting()
{
	EndText = TEXT("승리!!");
}

void UBattleResultWidget::LoserSetting()
{
	EndText = TEXT("패배!!");
}

void UBattleResultWidget::RequestRestart()
{
	EndText = TEXT("재대결 요청중...");
	// 게임모드에 재대결 요청 넘기기 -> 게임모드에 요청을 넘기려면 플레이어에 서버 함수를 만들고 거기서 요청
	Me->ServerRestartGame();
	RestartBtn->OnClicked.Clear();
	ExitBtn->OnClicked.Clear();

}

void UBattleResultWidget::ExitToLobby()
{
	// 게임모드에 로비로 나가기 요청 -> 게임모드에 요청을 넘기려면 플레이어에 서버 함수를 만들고 거기서 요청
	Me->ServerExitGame();
	RestartBtn->OnClicked.Clear();
	ExitBtn->OnClicked.Clear();
}

void UBattleResultWidget::SetEndText(FString NewText)
{
	EndText = NewText;
}
