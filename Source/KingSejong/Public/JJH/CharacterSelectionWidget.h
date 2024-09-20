// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterSelectionWidget.generated.h"

/**
 * 
 */
UCLASS()
class KINGSEJONG_API UCharacterSelectionWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USkeletalMesh* SelectedMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMesh* ManMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMesh* WomanMesh;

	UFUNCTION(BlueprintCallable, Category = "Character")
	void UpdateSelectedMesh(USkeletalMesh* NewMesh);

	UPROPERTY(meta = (BindWidget))
	class UButton* ManMeshButton;	
	
	UPROPERTY(meta = (BindWidget))
	class UButton* WomanMeshButton;	
	
	UPROPERTY(meta = (BindWidget))
	class UButton* SelectButton;

	virtual void NativeConstruct() override;
	UFUNCTION()
	void SelectButtonClicked();
	UFUNCTION()
	void ManMeshButtonClicked();
	UFUNCTION()
	void WomanMeshButtonClicked();
	
	void OnMeshSelected(USkeletalMesh* Mesh);
};
