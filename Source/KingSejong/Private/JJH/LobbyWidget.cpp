// Fill out your copyright notice in the Description page of Project Settings.


#include "JJH/LobbyWidget.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Components/CheckBox.h"
#include "Components/WidgetSwitcher.h"
#include "JJH/JJH_GameInstance.h"

void ULobbyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Host->OnClicked.AddDynamic(this, &ULobbyWidget::GoToCreateSessionUI);

	Join->OnClicked.AddDynamic(this, &ULobbyWidget::JoinServer);

	RunCheckBox->OnCheckStateChanged.AddDynamic(this, &ULobbyWidget::OnRunCheckBoxChecked);
	TalkCheckBox->OnCheckStateChanged.AddDynamic(this, &ULobbyWidget::OnTalkCheckBoxChecked);
	BattleCheckBox->OnCheckStateChanged.AddDynamic(this, &ULobbyWidget::OnBattleCheckBoxChecked);

	CreateSessionButton->OnClicked.AddDynamic(this, &ULobbyWidget::CreateSession);
}

void ULobbyWidget::SetMenuInterface(ILobbyInterface* LobbyInterface)
{
	this->WidgetLobbyInterface = LobbyInterface;
}

void ULobbyWidget::OnRunCheckBoxChecked(bool bIsChecked)
{
	if ( bIsChecked )
	{
		UncheckOthers(RunCheckBox);
	}
}

void ULobbyWidget::OnTalkCheckBoxChecked(bool bIsChecked)
{
	if ( bIsChecked )
	{
		UncheckOthers(TalkCheckBox);
	}
}

void ULobbyWidget::OnBattleCheckBoxChecked(bool bIsChecked)
{
	if ( bIsChecked )
	{
		UncheckOthers(BattleCheckBox);
	}
}

void ULobbyWidget::UncheckOthers(UCheckBox* CheckedBox)
{
	TArray<UCheckBox*> CheckBoxs = { RunCheckBox, TalkCheckBox, BattleCheckBox};
	for ( UCheckBox* Boxs : CheckBoxs )
	{
	//for문 돌면서 다른 박스들 체크 해제하기
		if (Boxs != CheckedBox)
		{
			Boxs->SetCheckedState(ECheckBoxState::Unchecked);
		}
	}
}

void ULobbyWidget::GoToCreateSessionUI()
{
	MenuSwitcher->SetActiveWidgetIndex(1);
}

void ULobbyWidget::CreateSession()
{
	if ( WidgetLobbyInterface)
	{
		UE_LOG(LogTemp, Warning, TEXT("ASD"));

		auto* gi = Cast<UJJH_GameInstance>(GetWorld()->GetGameInstance());
		//roomname은 체크해줘야함 비속어 이런거 ->fd dsf

		//
		FString roomName = RoomNameText->GetText().ToString();
		roomName = roomName.TrimStartAndEnd();
		if ( roomName.IsEmpty() )
		{
			return;
		}
		//int32 PlayerCount = ( int32 ) CR_Slider_PlayerCount->GetValue();
		//일단 4로 진행
		gi->CreateSession(roomName, 4);
	}
}

void ULobbyWidget::JoinServer()
{
	if ( WidgetLobbyInterface != nullptr )
	{
		if ( !ensure(IPAddress != nullptr) ) return;
		const FString & Address = IPAddress->GetText().ToString();
		//WidgetLobbyInterface->Join(Address);
	}	
}
