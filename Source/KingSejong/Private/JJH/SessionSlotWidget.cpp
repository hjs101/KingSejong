// Fill out your copyright notice in the Description page of Project Settings.

#include "JJH/SessionSlotWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "JJH/LobbyWidget.h"
#include "JJH/JJH_GameInstance.h"

void USessionSlotWidget::NativeConstruct()
{
	
}

void USessionSlotWidget::Setup(class ULobbyWidget* InParent)
{
	Parent = InParent;
	RowButton->OnClicked.AddDynamic(this, &USessionSlotWidget::OnClicked);
}

void USessionSlotWidget::UpdateInfo(const FRoomInfo& info)
{
	Text_RoomName->SetText(FText::FromString(info.roomName));
	Text_HostName->SetText(FText::FromString(info.hostName));
	FString Count = FString::Printf(TEXT("%d / %d"), info.CurrentPlayerCount, info.MaxPlayerCount);

	Text_PlayerCount->SetText(FText::FromString(Count));

	SessionSearchIndex = info.index;
}
//클릭하면 메인메뉴로 인덱스 보내기
void USessionSlotWidget::OnClicked()
{
	Parent->LobbySelectedIndex = SessionSearchIndex;
}