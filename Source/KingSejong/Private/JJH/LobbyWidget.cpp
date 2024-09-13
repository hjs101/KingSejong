// Fill out your copyright notice in the Description page of Project Settings.


#include "JJH/LobbyWidget.h"
#include "Components/Button.h"
#include "Components/EditableText.h"

bool ULobbyWidget::Initialize()
{
	bool Success = Super::Initialize();
	if ( !Success ) return false;

	if ( !ensure(Host != nullptr) ) return false;
	Host->OnClicked.AddDynamic(this, &ULobbyWidget::HostServer);

	if ( !ensure(Join != nullptr) ) return false;
	Join->OnClicked.AddDynamic(this, &ULobbyWidget::JoinServer);

	return true;
}

void ULobbyWidget::SetMenuInterface(ILobbyInterface* LobbyInterface)
{
	this->WidgetLobbyInterface = LobbyInterface;
}

void ULobbyWidget::HostServer()
{
	if ( WidgetLobbyInterface)
	{
		UE_LOG(LogTemp, Warning, TEXT("ASD"));
		WidgetLobbyInterface->Host();
	}
}

void ULobbyWidget::JoinServer()
{
	if ( WidgetLobbyInterface != nullptr )
	{
		if ( !ensure(IPAddress != nullptr) ) return;
		const FString & Address = IPAddress->GetText().ToString();
		WidgetLobbyInterface->Join(Address);
	}

	
}
