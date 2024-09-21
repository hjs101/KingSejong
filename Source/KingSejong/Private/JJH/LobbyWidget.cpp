// Fill out your copyright notice in the Description page of Project Settings.


#include "JJH/LobbyWidget.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Components/CheckBox.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "JJH/JJH_GameInstance.h"
#include "JJH/SessionSlotWidget.h"

void ULobbyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//게임인스턴스를 가져와서
	//AddSessionSlotWidget 를 델리게이트에 연결하기
	UJJH_GameInstance* gi = Cast<UJJH_GameInstance>(GetWorld()->GetGameInstance());
	gi->OnSearchSignatureCompleteDelegate.AddDynamic(this, &ULobbyWidget::AddSessionSlotWidget);

	Host->OnClicked.AddDynamic(this, &ULobbyWidget::GoToCreateSessionUI);

	Join->OnClicked.AddDynamic(this, &ULobbyWidget::JoinServer);

	FindButton->OnClicked.AddDynamic(this, &ULobbyWidget::OnFindButtonClicked);
	//체크박스
	RunCheckBox->OnCheckStateChanged.AddDynamic(this, &ULobbyWidget::OnRunCheckBoxChecked);
	TalkCheckBox->OnCheckStateChanged.AddDynamic(this, &ULobbyWidget::OnTalkCheckBoxChecked);
	BattleCheckBox->OnCheckStateChanged.AddDynamic(this, &ULobbyWidget::OnBattleCheckBoxChecked);
	//세션 만들기
	CreateSessionButton->OnClicked.AddDynamic(this, &ULobbyWidget::CreateSession);

	PlayerCountSlider->OnValueChanged.AddDynamic(this, &ULobbyWidget::OnSliderValueChanged);
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

		UJJH_GameInstance* gi = Cast<UJJH_GameInstance>(GetWorld()->GetGameInstance());
		//roomname은 체크해줘야함 비속어 이런거 ->fd dsf

		//
		FString roomName = RoomNameText->GetText().ToString();
		roomName = roomName.TrimStartAndEnd();
		if ( roomName.IsEmpty() )
		{
			return;
		}
		int32 PlayerCount = ( int32 ) PlayerCountSlider->GetValue();
		//일단 4로 진행
		gi->CreateSession(roomName, PlayerCount, WidgetCategory);
	}
}

void ULobbyWidget::JoinServer()
{
	if (WidgetLobbyInterface)
	{
		UE_LOG(LogTemp, Warning, TEXT("hhhD"));
		WidgetLobbyInterface->JoinToSession(LobbySelectedIndex);
	}	
}

void ULobbyWidget::OnSliderValueChanged(float Value)
{
	int IntValue = FMath::FloorToInt(Value);
	PlayerCountText->SetText(FText::AsNumber(IntValue));
}


void ULobbyWidget::AddSessionSlotWidget(const FRoomInfo& info)
{
	//슬롯을 생성해서 스크롤박스에 붙이고 싶다.(추가하고 싶다)
	auto* slot = CreateWidget<USessionSlotWidget>(this, SessionSlotWidgetFactory);

	//슬롯에서 위젯에 뿌리기
	slot->UpdateInfo(info);
	//부모 기억시키기
	slot->Setup(this);
	//스크롤박스에 붙이기
	FS_ScrollBox->AddChild(slot);

}

void ULobbyWidget::OnFindButtonClicked()
{


	//기존 스크롤박스 목록 삭제하고
	if ( FS_ScrollBox && FS_ScrollBox->GetChildrenCount() == 0 )
	{
		UE_LOG(LogTemp, Warning, TEXT("hhhD2"));
	}
	FS_ScrollBox->ClearChildren();
	//방찾기를 수행
	UJJH_GameInstance* gi = Cast<UJJH_GameInstance>(GetWorld()->GetGameInstance());
	if ( gi )
	{
		gi->FindOtherSessions();
	}
}

void ULobbyWidget::SetTextAndCategory(const FString& Text, const FString& Category)
{
	BattleTypeText->SetText(FText::FromString(Text));
	WidgetCategory = Category;
}


