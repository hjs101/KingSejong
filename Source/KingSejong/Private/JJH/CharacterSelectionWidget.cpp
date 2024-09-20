// Fill out your copyright notice in the Description page of Project Settings.


#include "JJH/CharacterSelectionWidget.h"
#include "Components/Button.h"
#include "JJH/JJH_GameInstance.h"
#include "GameFramework/Character.h"
#include "JJH/JJHPlayerState.h"
#include "JJH/SelectPlayerInterface.h"

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
	OnMeshSelected(SelectedMesh);
	APlayerController* PC = GetOwningPlayer();
	if ( PC )
	{
		PC->SetInputMode(FInputModeGameOnly());
		PC->bShowMouseCursor = false;
		RemoveFromParent();
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

void UCharacterSelectionWidget::OnMeshSelected(USkeletalMesh* Mesh)
{
    //// 1. 위젯의 소유자(플레이어 컨트롤러) 가져오기
    //APlayerController* PlayerController = GetOwningPlayer();
    //if ( PlayerController )
    //{
    //    // 2. 플레이어 컨트롤러의 폰(캐릭터) 가져오기
    //    APawn* ControlledPawn = PlayerController->GetPawn();
    //    if ( ControlledPawn )
    //    {
    //        // 3. ISelectPlayerInterface 구현 여부 확인
    //        ISelectPlayerInterface* SelectablePlayer = Cast<ISelectPlayerInterface>(ControlledPawn);
    //        if ( SelectablePlayer )
    //        {
    //            // 4. UpdateMesh 함수 호출
    //            SelectablePlayer->UpdateMesh(Mesh);
    //        }
    //    }
    //}
	UJJH_GameInstance* GameInstance = Cast<UJJH_GameInstance>(GetGameInstance());
	if ( GameInstance )
	{
		GameInstance->UpdatePlayerMesh(GetOwningPlayer(), SelectedMesh);
	}
}