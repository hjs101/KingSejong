// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OpeningWidget.generated.h"

/**
 * 
 */
UCLASS()
class KINGSEJONG_API UOpeningWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	// Left, Right Button
	UFUNCTION()
	void OnLeft();
	UFUNCTION()
	void OnRight();
	UFUNCTION()
	void OnQuit();
	UPROPERTY(meta=(BindWidget))
	class UButton* RightBtn;
	UPROPERTY(meta=(BindWidget))
	UButton* LeftBtn;
	UPROPERTY(meta=(BindWidget))
	UButton* QuitBtn;

	UPROPERTY(meta=(BindWidget))
	class UImage* OpeningImg;

	UPROPERTY(EditDefaultsOnly)
	class UTexture2D* StoryTexture;
	UPROPERTY(EditDefaultsOnly)
	UTexture2D* RunningTexture;
	UPROPERTY(EditDefaultsOnly)
	UTexture2D* BattleTexture;
	UPROPERTY(EditDefaultsOnly)
	UTexture2D* TalkTexture;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<UTexture2D*> OpeningTextures;

	void ChangeImg(int32 Num);

	// 애니매트로닉스 사진 번호
	int32 PageNum = 0;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UCharaceterSelecteWidget> SelectFactory;

	UPROPERTY()
	class UCharaceterSelecteWidget* CharacterSelectWidget;
};
