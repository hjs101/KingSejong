// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharaceterSelecteWidget.generated.h"

/**
 * 
 */
UCLASS()
class KINGSEJONG_API UCharaceterSelecteWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere)
	TArray<class USkeletalMesh* > CharacterList;

	int32 index = 0;

	UPROPERTY()
	class ACharacter* Char;

	virtual void NativeConstruct() override;	

	UPROPERTY(meta = (BindWidget))
	class UButton* NextButton;	
	
	UPROPERTY(meta = (BindWidget))
	class UButton* PrevButton;	
	
	UPROPERTY(meta = (BindWidget))
	class UButton* SelectButton;

	UFUNCTION()
	void SelecteButtonClicked();	
	
	UFUNCTION()
	void NextButtonClicked();	
	
	UFUNCTION()
	void PrevButtonClicked();



};
