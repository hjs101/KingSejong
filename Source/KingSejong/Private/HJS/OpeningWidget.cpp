// Fill out your copyright notice in the Description page of Project Settings.


#include "HJS/OpeningWidget.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "JJH/CharaceterSelecteWidget.h"
void UOpeningWidget::NativeConstruct()
{
	Super::NativeConstruct();
	// 텍스쳐 초기화 4개.
	OpeningTextures.Add(StoryTexture);
	OpeningTextures.Add(RunningTexture);
	OpeningTextures.Add(BattleTexture);
	OpeningTextures.Add(TalkTexture);

	if (OpeningTextures.Num() > 0)
	{
		// 첫 이미지는 프레디
		ChangeImg(PageNum);
	}
	LeftBtn->SetVisibility(ESlateVisibility::Hidden);

	LeftBtn->OnClicked.AddDynamic(this, &UOpeningWidget::OnLeft);
	RightBtn->OnClicked.AddDynamic(this, &UOpeningWidget::OnRight);
	QuitBtn->OnClicked.AddDynamic(this, &UOpeningWidget::OnQuit);
}

void UOpeningWidget::OnLeft()
{
	// 3일때 오른쪽 버튼 켜주기
	if (PageNum == 3)
	{
		RightBtn->SetVisibility(ESlateVisibility::Visible);
	}

	// 1일 때 왼쪽 버튼 꺼주기
	if (PageNum == 1)
	{
		LeftBtn->SetVisibility(ESlateVisibility::Hidden);
	}
	// Num을 하나씩 빼주고
	PageNum--;
	// 방어코드
	if (PageNum < 0)
	{
		return;
	}
	ChangeImg(PageNum);
}

void UOpeningWidget::OnRight()
{
	// 0일때 왼쪽 버튼 켜주기
	if (PageNum == 0)
	{
		LeftBtn->SetVisibility(ESlateVisibility::Visible);
	}

	// 2일 때 오른쪽 버튼 꺼주기
	if (PageNum == 2)
	{
		RightBtn->SetVisibility(ESlateVisibility::Hidden);
	}

	// 번호를 1 더해주고
	PageNum++;
	// 방어코드
	if (PageNum >= OpeningTextures.Num())
	{
		return;
	}
	ChangeImg(PageNum);
}

void UOpeningWidget::OnQuit()
{
	SetVisibility(ESlateVisibility::Hidden);
	CharacterSelectWidget = CreateWidget<UCharaceterSelecteWidget>(GetWorld(),SelectFactory);
	check(CharacterSelectWidget);
	CharacterSelectWidget->AddToViewport();
}

void UOpeningWidget::ChangeImg(int32 Num)
{
	if (OpeningImg)
	{
		// 이미지 변경해주기
		OpeningImg->SetBrushFromTexture(OpeningTextures[Num]);
	}
}