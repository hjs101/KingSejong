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
	virtual void NativeConstruct();
public:

	UPROPERTY(meta = (BindWidget))
	class UButton* Host;

	UPROPERTY(meta = (BindWidget))
	class UButton* Join;	
	UPROPERTY(meta = (BindWidget))
	class UEditableText* IPAddress;

	void SetMenuInterface(ILobbyInterface* LobbyInterface);

	//체크박스
	UPROPERTY(meta = (BindWidget))
	class UCheckBox* RunCheckBox;	
	UPROPERTY(meta = (BindWidget))
	class UCheckBox* TalkCheckBox;	
	UPROPERTY(meta = (BindWidget))
	class UCheckBox* BattleCheckBox;
	//위젯 스위쳐
	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* MenuSwitcher;
	//방제
	UPROPERTY(meta = (BindWidget))
	class UEditableText* RoomNameText;
	//방만들기버튼
	UPROPERTY(meta = (BindWidget))
	class UButton* CreateSessionButton;

	UFUNCTION()
	void OnRunCheckBoxChecked(bool bIsChecked);
	UFUNCTION()
	void OnTalkCheckBoxChecked(bool bIsChecked);
	UFUNCTION()
	void OnBattleCheckBoxChecked(bool bIsChecked);

	void UncheckOthers(UCheckBox* CheckedBox);

	UFUNCTION()
	void GoToCreateSessionUI();

	UFUNCTION()
	void CreateSession();
	UFUNCTION()
	void JoinServer();
	ILobbyInterface* WidgetLobbyInterface;


};
