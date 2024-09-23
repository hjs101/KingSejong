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
	UFUNCTION()
	void OnQuitButtonClicked();

	UPROPERTY(meta = (BindWidget))
	class UButton* Host;

	UPROPERTY(meta = (BindWidget))
	class UButton* Join;		
	
	UPROPERTY(meta = (BindWidget))
	class UButton* FindButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* QuitButton;	
	UPROPERTY(meta = (BindWidget))
	class UButton* QuitButton_1;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* BattleTypeText;

	UPROPERTY(EditAnywhere)
	FString WidgetCategory;

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
	//인원제한 슬라이더	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* PlayerCountText;
	
	UPROPERTY(meta = (BindWidget))
	class USlider* PlayerCountSlider;	
	//스크롤 박스
	UPROPERTY(meta = (BindWidget))
	class UScrollBox* FS_ScrollBox;	


	//체크박스 체크되었을때
	UFUNCTION()
	void OnRunCheckBoxChecked(bool bIsChecked);
	UFUNCTION()
	void OnTalkCheckBoxChecked(bool bIsChecked);
	UFUNCTION()
	void OnBattleCheckBoxChecked(bool bIsChecked);
	//다른것들 체크해제
	void UncheckOthers(UCheckBox* CheckedBox);


	UFUNCTION()
	void GoToCreateSessionUI();

	UFUNCTION()
	void CreateSession();
	UFUNCTION()
	void JoinServer();

	UFUNCTION()
	void OnSliderValueChanged(float Value);
	
	UFUNCTION()
	void AddSessionSlotWidget(const FRoomInfo& info);

	UFUNCTION()
	void OnFindButtonClicked();

	ILobbyInterface* WidgetLobbyInterface;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class USessionSlotWidget>  SessionSlotWidgetFactory;

	UPROPERTY(EditAnywhere)
	int32 LobbySelectedIndex = -1;

	void SetTextAndCategory(const FString& Text, const FString& Category);

};
