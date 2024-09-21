// Fill out your copyright notice in the Description page of Project Settings.


#include "HJS/MainUI.h"
#include "components/Image.h"
#include "components/TextBlock.h"
#include "components/Border.h"
#include "HJS/HJS_BattlePlayer.h"
#include "HJS/BattleQuestionStruct.h"
#include "HJS/BattleResultWidget.h"

void UMainUI::NativeConstruct()
{
	Super::NativeConstruct();

	LineText = TEXT("");
	CountDown->SetVisibility(ESlateVisibility::Hidden);

	ResultUI->SetVisibility(ESlateVisibility::Hidden);

	TutorialTextArr.Add(TEXT("이 말하기 대결, 나 훈장이 참관하겠네."));
	TutorialTextArr.Add(TEXT("이 서당의 공식 대결 규칙으로 진행하겠다."));
	TutorialTextArr.Add(TEXT("빈 칸에 들어갈 알맞은 단어를 보기에서 골라 조합하여 읽어야 한다. 각자의 어휘력과 발음을 겨룰 수 있는 최고의 규칙이지."));
	TutorialTextArr.Add(TEXT("제시된 문장에 맞춰 가장 어울리는 단어를 선택해 말해야 해."));
	TutorialTextArr.Add(TEXT("모두 준비가 될 때까지 잠시 기다리도록, 3초를 센 후에 문제가 나타날 것이야."));
	CountDownText = TEXT("3");

	Answer1 = TEXT("가가가");
	Answer2 = TEXT("가가가");
	Answer3 = TEXT("가가가");
	Answer4 = TEXT("가가가");
}

void UMainUI::SetHP(int32 PlayerNum , int32 CurrentHP)
{

	if ( PlayerNum == 1 )
	{
		if ( CurrentHP == 2 )
		{
			Player1_HP3->SetVisibility(ESlateVisibility::Hidden);
			return;
		}

		if ( CurrentHP == 1 )
		{
			Player1_HP3->SetVisibility(ESlateVisibility::Hidden);
			Player1_HP2->SetVisibility(ESlateVisibility::Hidden);
			return;
		}

		if ( CurrentHP == 0 )
		{
			Player1_HP3->SetVisibility(ESlateVisibility::Hidden);
			Player1_HP2->SetVisibility(ESlateVisibility::Hidden);
			Player1_HP1->SetVisibility(ESlateVisibility::Hidden);
			return;
		}
	}
	else
	{
		if ( CurrentHP == 2 )
		{
			Player2_HP3->SetVisibility(ESlateVisibility::Hidden);
			return;
		}

		if ( CurrentHP == 1 )
		{
			Player2_HP3->SetVisibility(ESlateVisibility::Hidden);
			Player2_HP2->SetVisibility(ESlateVisibility::Hidden);
			return;
		}

		if ( CurrentHP == 0 )
		{
			Player2_HP3->SetVisibility(ESlateVisibility::Hidden);
			Player2_HP2->SetVisibility(ESlateVisibility::Hidden);
			Player2_HP1->SetVisibility(ESlateVisibility::Hidden);
			return;
		}
	}

}

void UMainUI::GameStartUIInit()
{
	CountDown->SetVisibility(ESlateVisibility::Hidden);
	OptionCanvas->SetVisibility(ESlateVisibility::Hidden);
}

void UMainUI::TurnStartUIInit()
{
	CountDown->SetVisibility(ESlateVisibility::Visible);
	GetWorld()->GetTimerManager().SetTimer(CountDownTimerHandle , this , &UMainUI::OnCountDown , 1.f , true);
}

void UMainUI::OnCountDown()
{
	if ( CountDownText.Equals(TEXT("3")))
	{
		CountDownText = TEXT("2");
	}else if ( CountDownText.Equals(TEXT("2")) )
	{
		CountDownText = TEXT("1");
	}else if ( CountDownText.Equals(TEXT("1")) )
	{
		GetWorld()->GetTimerManager().ClearTimer(CountDownTimerHandle);
		Me->ServerQuestionSetting();
		CountDown->SetVisibility(ESlateVisibility::Hidden);
		CountDownText = TEXT("3");
	}
}

void UMainUI::RecordStartUIInit(FBattleQuestionData Data)
{
	
	Answer1 = Data.Answer1;
	Answer2 = Data.Answer2;
	Answer3 = Data.Answer3;
	Answer4 = Data.Answer4;
	LineText = Data.Question;
	
	OptionCanvas->SetVisibility(ESlateVisibility::Visible);

}

void UMainUI::SetTeacherLine(FString Text , int32 TeacherMode)
{
	
	check(Idle);
	check(Smile);
	check(Angry);

	// 0은 일반 1은 화남 2는 웃음
	switch ( TeacherMode )
	{
	case 0:
		Teacher->SetBrushFromTexture(Idle);
		break;
	case 1:
		Teacher->SetBrushFromTexture(Angry);
		break;
	case 2:
		Teacher->SetBrushFromTexture(Smile);
		break;
	}

	//LineText = Text;

	

}

void UMainUI::SetOptions(TArray<FString> OptionsArr)
{
	Answer1 = OptionsArr[0];
	Answer2 = OptionsArr[1];
	Answer3 = OptionsArr[2];
	Answer4 = OptionsArr[3];
}



void UMainUI::SetTextToDisplay(const FString& NewText)
{
	FullText = NewText;
	CurrentText = TEXT("");
	GetWorld()->GetTimerManager().ClearTimer(TextAnimTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(TextAnimTimerHandle , this , &UMainUI::UpdateText , TextSpeed , true);
}

void UMainUI::UpdateText()
{
	if ( CurrentText.Len() < FullText.Len() )
	{
		// 다음 글자를 추가
		CurrentText += FullText.Mid(CurrentText.Len() , 1);
		// TextBlock에 업데이트된 텍스트 설정
		LineText = CurrentText;
	}
	else
	{
		// 모든 텍스트가 표시되면 남은 대사가 있는 지 체크
		if ( CurrentNum+1 < TutorialTextArr.Num() )
		{
			// 다 표시되면 클리어 타이머
			GetWorld()->GetTimerManager().ClearTimer(TextAnimTimerHandle);
			// 다음 대사가 있으면 다음 대사를 체크
			GetWorld()->GetTimerManager().SetTimer(NextTextTimerHandle , this , &UMainUI::NextTextSet , NextTextStartRate , false);
			
		}
		else
		{
			// 모든 튜토리얼 재생이 완료되면 서버쪽에 완료됐음을 알려야 함
			GetWorld()->GetTimerManager().ClearTimer(TextAnimTimerHandle);
			check(Me);
			Me->ServerStartTurnToGM();
		}
	}
}

void UMainUI::StartTutorialUI(int32 Num)
{
	CurrentNum = Num;
	SetTextToDisplay(TutorialTextArr[Num]);
}

void UMainUI::NextTextSet()
{
	SetTextToDisplay(TutorialTextArr[++CurrentNum]);
}

void UMainUI::ShowEndGameUI(bool bWin)
{
	ResultUI->SetVisibility(ESlateVisibility::Visible);
	if (bWin)
	{
		ResultUI->WinnerSetting();
	}
	else
	{
		ResultUI->LoserSetting();
	}
}

void UMainUI::SettingEndGameUIText(const FString& NewText)
{
	check(ResultUI);
	ResultUI->SetEndText(NewText);
}

void UMainUI::SettingPlayer(AHJS_BattlePlayer* Player)
{
	Me = Player;
	ResultUI->Me = Player;
}
