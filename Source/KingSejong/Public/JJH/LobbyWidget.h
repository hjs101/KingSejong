// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyInterface.h"
#include "LobbyWidget.generated.h"

/**
 * 
 */
UCLASS()
class KINGSEJONG_API ULobbyWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual bool Initialize();
public:

	UPROPERTY(meta = (BindWidget))
	class UButton* Host;

	UPROPERTY(meta = (BindWidget))
	class UButton* Join;	
	UPROPERTY(meta = (BindWidget))
	class UEditableText* IPAddress;

	void SetMenuInterface(ILobbyInterface* LobbyInterface);

	UFUNCTION()
	void HostServer();
	UFUNCTION()
	void JoinServer();
	ILobbyInterface* WidgetLobbyInterface;
};
