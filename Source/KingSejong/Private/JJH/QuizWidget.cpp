// Fill out your copyright notice in the Description page of Project Settings.

#include "JJH/QuizWidget.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/HorizontalBox.h"
#include "Components/Overlay.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Image.h"
#include "Engine/Texture2D.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Styling/SlateBrush.h"
#include "UObject/ConstructorHelpers.h"
#include "Slate/SlateBrushAsset.h"
#include "JJH/RunnerController.h"
#include "JJH/RunningGameModeBase.h"
#include "JJH/JJH_GameInstance.h"

void UQuizWidget::InitializeQuiz(const FWordsData& WordData)
{
	CurrentWordData = WordData;
	
	//선생님 멘트
	TeacherSpeak->SetVisibility(ESlateVisibility::Visible);
	//퀴즈창 숨기기
	Quiz->SetVisibility(ESlateVisibility::Hidden);

	//3초후에 초성 보여주기 -> 후에 바꿔야함
	GetWorld()->GetTimerManager().SetTimer(ShowInitialTimerHandle, this, &UQuizWidget::ShowInitials, 2.f, false);

	//카운트다운 넘버 설정
	CountDownText->SetText(FText::FromString(FString::FromInt(CountDownNum)));
	CountDownOverlay->SetVisibility(ESlateVisibility::Hidden);

	//웃는 훈장
	FString AssetPath = TEXT("/Game/JJH/UI/SmileTeacher");
	SmileTeacher = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass() , nullptr , *AssetPath));
	 
	//화난 훈장
	FString AssetPath2 = TEXT("/Game/JJH/UI/AngryTeacher");
	AngryTeacher = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass() , nullptr , *AssetPath2));

	PlayAnimation(TeacherIntro);
}

void UQuizWidget::ShowInitials()
{
	//로딩창 숨기기
	TeacherSpeak->SetVisibility(ESlateVisibility::Hidden);
	//퀴즈창 보여주기
	Quiz->SetVisibility(ESlateVisibility::Visible);
	//초성 보여주기
	Initials->SetText(FText::FromString(CurrentWordData.Initials));
	Initials->SetVisibility(ESlateVisibility::Visible);
	//뜻 보여주기
	Meaning->SetText(FText::FromString(CurrentWordData.Meaning));
	Meaning->SetVisibility(ESlateVisibility::Visible);
	//두루마리 보여주기
	WhiteImage->SetVisibility(ESlateVisibility::Visible);
	ScrollImage->SetVisibility(ESlateVisibility::Visible);

	GetWorld()->GetTimerManager().ClearTimer(ShowInitialTimerHandle);

}

void UQuizWidget::ShowAnswerTextBox()
{
	QuizLoading->SetVisibility(ESlateVisibility::Hidden);
	
	AnswerHorizontal->SetVisibility(ESlateVisibility::Visible);
	Meaning->SetVisibility(ESlateVisibility::Visible);
	Initials->SetVisibility(ESlateVisibility::Visible);
	//두루마리 보여주기
	WhiteImage->SetVisibility(ESlateVisibility::Visible);
	ScrollImage->SetVisibility(ESlateVisibility::Visible);
}

void  UQuizWidget::HideLoading()
{
	QuizLoading->SetVisibility(ESlateVisibility::Hidden);
	TeacherSpeak->SetVisibility(ESlateVisibility::Hidden);
}
void UQuizWidget::ShowLoading()
{
	QuizLoading->SetVisibility(ESlateVisibility::Visible);
}
void UQuizWidget::StartCountDown()
{
	// 타이머 시작
	GetWorld()->GetTimerManager().SetTimer(CountDownTimerHandle, this, &UQuizWidget::UpdateCountDown, 1.0f, true);
	CountDownOverlay->SetVisibility(ESlateVisibility::Visible);
	CountDownText->SetVisibility(ESlateVisibility::Visible);
}

void UQuizWidget::UpdateCountDown()
{
	//불값하나 만들어서 답 입력하면 사라지게끔해야함
	//카운트다운넘버 블루프린트에서 세팅해놓기
	if (CountDownNum > 0)
	{
		CountDownNum--;
		CountDownText->SetText(FText::FromString(FString::FromInt(CountDownNum)));
	}
	else
	{
		// 들어오면 종료텍스트 빼고 다 빼야함

		GetWorld()->GetTimerManager().ClearTimer(CountDownTimerHandle);

		QuizLoading->SetVisibility(ESlateVisibility::Hidden);
		CountDownText->SetText(FText::FromString(TEXT("종료")));
		Meaning->SetVisibility(ESlateVisibility::Hidden);
		Initials->SetVisibility(ESlateVisibility::Hidden);
		WhiteImage->SetVisibility(ESlateVisibility::Hidden);
		ScrollImage->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UQuizWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//정답칸 히든으로 숨겨놓기
	AnswerHorizontal->SetVisibility(ESlateVisibility::Hidden);
	//카운트다운
	CountDownText->SetVisibility(ESlateVisibility::Hidden);

	AnswerSubmitButton->OnClicked.AddDynamic(this, &UQuizWidget::SubmitAnswer);

	QuizLoading->SetVisibility(ESlateVisibility::Hidden);

	AnswerTextBox->OnTextChanged.AddDynamic(this, &UQuizWidget::OnAnswerTextChanged);
	//입력받을때마다 컨트롤러 불러서 입력한 텍스트 받아와서 게임모드 전달하기?
	//게임모드는 그거 받아오면 컨트롤러 일렬 불러서 UpdateTextBoxContent부르기.

	QuitButton->OnClicked.AddDynamic(this, &UQuizWidget::OnQuitButtonClicked);
}

void UQuizWidget::SubmitAnswer()
{
	if (AnswerTextBox)
	{
		FString UserAnswer = AnswerTextBox->GetText().ToString();
		APlayerController* PC = GetOwningPlayer();
		ARunnerController* RunnerController = Cast<ARunnerController>(PC);

		if ( RunnerController )
		{
			RunnerController->ServerSubmitAnswer(UserAnswer);
		}
		
		//AnswerHorizontal->SetVisibility(ESlateVisibility::Hidden);
		//if ( UserAnswer.Equals(Answer) )
		//{
		//	//맞추면 퀴즈창 없애고 몇초 이따가 레벨 초기화
		//	Quiz->SetVisibility(ESlateVisibility::Hidden);
		//	TeacherSpeak->SetVisibility(ESlateVisibility::Visible);
		//	//선생님 텍스처 바꾸기
		//	Teacher->SetBrushFromTexture(SmileTeacher);
		//	TeacherText->SetText(FText::FromString(TEXT("대단하구나!")));
		//}
		//else
		//{
		//	//틀리면 퀴즈창 유지하고 다른 사람한테 넘어가기?
		//	TeacherSpeak->SetVisibility(ESlateVisibility::Visible);
		//	Teacher->SetBrushFromTexture(AngryTeacher);
		//	TeacherText->SetText(FText::FromString(TEXT("아니다 욘석아")));
		//	PlayAnimation(TeacherAngry, 0, 1, EUMGSequencePlayMode::PingPong);
		//	
		//	// 다음 플레이어로 차례 넘기기
		//	// 컨트롤러에 접근하여 다음 플레이어로 넘기기
		//	APlayerController* PC = GetOwningPlayer();
		//	ARunnerController* RunnerController = Cast<ARunnerController>(PC);
		//	if ( RunnerController )
		//	{
		//		RunnerController->MoveToNextPlayerWithDelay();
		//	}
		//}
	}
}

void UQuizWidget::OnAnswerTextChanged(const FText& Text)
{
	ARunnerController* MyController = Cast<ARunnerController>(GetOwningPlayer());
	if ( MyController )
	{
		MyController->SubmitAnswerTextToServer(Text.ToString());  // 서버로 텍스트 전달
	}
}
void UQuizWidget::UpdateTextBoxContent(const FString& TextContent)
{
	CountDownText->SetText(FText::FromString(TextContent));
}

void UQuizWidget::ShowTeacherSpeak(bool bIsCorrect)
{
	if (!IsValid(TeacherSpeak) || !IsValid(Teacher) || !IsValid(TeacherText))
	{
		UE_LOG(LogTemp, Error, TEXT("One or more widget components are invalid in ShowTeacherSpeak"));
		return;
	}
	TeacherSpeak->SetVisibility(ESlateVisibility::Visible);
	FTimerHandle TeacherSpeakTimer;

	if ( bIsCorrect )
	{
		//Teacher->SetBrushFromTexture(SmileTeacher);
		TeacherText->SetText(FText::FromString(TEXT("대단하구나!")));

	}
	else
	{
		//Teacher->SetBrushFromTexture(AngryTeacher);
		TeacherText->SetText(FText::FromString(TEXT("아니다 욘석아")));
		PlayAnimation(TeacherAngry , 0 , 1 , EUMGSequencePlayMode::PingPong);
		GEngine->AddOnScreenDebugMessage(-1 , 5.f , FColor::Blue , FString::Printf(TEXT("false")));
	}

	GetWorld()->GetTimerManager().SetTimer(TeacherSpeakTimer, this, &UQuizWidget::HideTeacherSpeak, 2.0f, false);
}

void UQuizWidget::HideTeacherSpeak()
{
	TeacherSpeak->SetVisibility(ESlateVisibility::Hidden);

}

void UQuizWidget::HideAnswerText()
{
	AnswerHorizontal->SetVisibility(ESlateVisibility::Hidden);
}

void UQuizWidget::SwitchToEnd()
{
	QuizLoadingSwitcher->SetActiveWidgetIndex(1);
}

void UQuizWidget::OnQuitButtonClicked()
{
	UJJH_GameInstance* gi = Cast<UJJH_GameInstance>(GetWorld()->GetGameInstance());
	if (gi)
	{
		gi->ExitSession();
	}
}
void UQuizWidget::OnRestartButtonClicked()
{

}