// Fill out your copyright notice in the Description page of Project Settings.


#include "JJH/CharacterSelectionWidget.h"
#include "Components/Button.h"
#include "JJH/JJH_GameInstance.h"
#include "GameFramework/Character.h"

void UCharacterSelectionWidget::UpdateSelectedMesh(USkeletalMesh* NewMesh)
{
	SelectedMesh = NewMesh;
}

void UCharacterSelectionWidget::NativeConstruct()
{
	ManMeshButton->OnClicked.AddDynamic(this, &UCharacterSelectionWidget::ManMeshButtonClicked);
	WomanMeshButton->OnClicked.AddDynamic(this, &UCharacterSelectionWidget::WomanMeshButtonClicked);
	SelectButton->OnClicked.AddDynamic(this, &UCharacterSelectionWidget::SelectButtonClicked);
}

void UCharacterSelectionWidget::SelectButtonClicked()
{
	if ( UJJH_GameInstance* gi = Cast<UJJH_GameInstance>(GetGameInstance()) )
	{
		gi->SetSelectedCharacterMesh(SelectedMesh);
	}
    // UI 제거
    RemoveFromParent();

    // 게임플레이 모드로 전환
    APlayerController* PC = GetOwningPlayer();
    if ( PC )
    {
        PC->SetInputMode(FInputModeGameOnly());
        PC->bShowMouseCursor = false;

        // 캐릭터 스폰 또는 메시 업데이트
        ACharacter* MyCharacter = Cast<ACharacter>(PC->GetPawn());
        if ( MyCharacter )
        {
            MyCharacter->GetMesh()->SetSkeletalMesh(SelectedMesh);
        }
    }
}

void UCharacterSelectionWidget::ManMeshButtonClicked()
{
	UpdateSelectedMesh(ManMesh);
}

void UCharacterSelectionWidget::WomanMeshButtonClicked()
{
	UpdateSelectedMesh(WomanMesh);
}