// Fill out your copyright notice in the Description page of Project Settings.


#include "JJH/CharaceterSelecteWidget.h"
#include "Components/Button.h"
#include "GameFramework/Character.h"

void UCharaceterSelecteWidget::NativeConstruct()
{
	SelectButton->OnClicked.AddDynamic(this, &UCharaceterSelecteWidget::SelecteButtonClicked);
	NextButton->OnClicked.AddDynamic(this, &UCharaceterSelecteWidget::NextButtonClicked);
	PrevButton->OnClicked.AddDynamic(this, &UCharaceterSelecteWidget::PrevButtonClicked);

	AController* Player = GetOwningPlayer();
	Char = Player->GetCharacter();
	
}

void UCharaceterSelecteWidget::SelecteButtonClicked()
{
	//CharacterList[index]를 게임인스턴스에 전달해주고 게임 시작하기 

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
