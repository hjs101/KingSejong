// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SessionSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class KINGSEJONG_API USessionSlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_RoomName;	

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_HostName;	
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_PlayerCount;	
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_PingMS;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* RowButton;

	int32 SessionSearchIndex;

	virtual void NativeConstruct() override;

	void Setup(class ULobbyWidget* InParent);
	//구조체의 전방선언은 앞에 struct붙임
	void UpdateInfo(const struct FRoomInfo& info);

	UPROPERTY()
	class ULobbyWidget* Parent;

	UFUNCTION()
	void OnClicked();
};
