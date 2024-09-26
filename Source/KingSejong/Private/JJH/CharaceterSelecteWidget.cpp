// Fill out your copyright notice in the Description page of Project Settings.


#include "JJH/CharaceterSelecteWidget.h"
#include "Components/Button.h"
#include "GameFramework/Character.h"
#include "JJH/JJH_GameInstance.h"
#include "JJH/JJHPlayerState.h"
#include "GameFramework/SpringArmComponent.h"

void UCharaceterSelecteWidget::NativeConstruct()
{
	SelectButton->OnClicked.AddDynamic(this, &UCharaceterSelecteWidget::SelecteButtonClicked);
	NextButton->OnClicked.AddDynamic(this, &UCharaceterSelecteWidget::NextButtonClicked);
	PrevButton->OnClicked.AddDynamic(this, &UCharaceterSelecteWidget::PrevButtonClicked);

	AController* Player = GetOwningPlayer();
	Char = Player->GetCharacter();
	PlayAnimation(ChooseCharacterAnim);
}

void UCharaceterSelecteWidget::SelecteButtonClicked()
{
	//CharacterList[index]를 게임인스턴스에 전달해주고 게임 시작하기 
	//플레이어 스테이트에서 얻기
	//AJJHPlayerState* PS = Cast<AJJHPlayerState>(Char->GetPlayerState());
	//PS->SelectedMeshIndex = index;
	UJJH_GameInstance* GI = Cast<UJJH_GameInstance>(GetWorld()->GetGameInstance());
	GI->CharacterMeshIndex = index;

	SetVisibility(ESlateVisibility::Hidden);

	auto* PC = GetWorld()->GetFirstPlayerController();
	PC->SetShowMouseCursor(false);
	PC->SetInputMode(FInputModeGameOnly());
	Cast<USpringArmComponent>(Char->GetComponentsByTag(USpringArmComponent::StaticClass(),FName(TEXT("PlayerCamera")))[0])->bUsePawnControlRotation = true;

	UUserWidget* LobbyMain = CreateWidget<UUserWidget>(GetWorld(),LobbyMainFactory);
	if (LobbyMain)
	{
		LobbyMain->AddToViewport();
	}

}

void UCharaceterSelecteWidget::NextButtonClicked()
{
	if (index >= CharacterList.Num() - 1)
	{
		index = 0;
	}
	else
	{
		index++;
	}
	Char->GetMesh()->SetSkeletalMesh(CharacterList[index]);
}

void UCharaceterSelecteWidget::PrevButtonClicked()
{
	if (index == 0)
	{
		index = CharacterList.Num() - 1;
	}
	else
	{
		index--;
	}
	Char->GetMesh()->SetSkeletalMesh(CharacterList[index]);
}
