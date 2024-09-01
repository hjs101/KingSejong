// Fill out your copyright notice in the Description page of Project Settings.


#include "JJH/RunningGameModeBase.h"
#include "JJH/RunningGameInstance.h"
#include "JJH/QuizWidget.h"
#include "JJH/RunnerController.h"

void ARunningGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle QuizTimer;
	//3초이따 퀴즈띄우기
	GetWorld()->GetTimerManager().SetTimer(QuizTimer, this, &ARunningGameModeBase::StartQuiz, 3.f, false);

}

void ARunningGameModeBase::StartQuiz()
{
	//랜덤 데이터 가져오기
	FWordsData SelectedQuiz = SelectRandomQuizData();

	//가져온데이터로 퀴즈뿌리기
	MulticastSendQuizData(SelectedQuiz);
}

void ARunningGameModeBase::PlayerCrossedFinishLine(APlayerController* PlayerController)
{
	if (PlayerFinishOrder.Num() == 0) // 첫 번째 플레이어가 결승선에 도착했을 때
	{
		MulticastStartCountdownTimer();
	}
	PlayerFinishOrder.Add(PlayerController);
}

void ARunningGameModeBase::MulticastStartCountdownTimer_Implementation()
{

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		ARunnerController* MyPC = Cast<ARunnerController>(It->Get());
		if (MyPC)
		{
			MyPC->ClientStartWidgetCountDown();  // 클라이언트의 카운트다운 UI 시작
		}
	}
}

FWordsData ARunningGameModeBase::SelectRandomQuizData()
{
	if (!WordsDataTable)
	{
		//데이터 테이블이 없으면 빈 구조체 인스턴스 반환
		return FWordsData();
	}

	TArray<FName> RowNames = WordsDataTable->GetRowNames();
	int32 RowCount = RowNames.Num();

	if (RowCount == 0)
	{	
		//데이터 테이블안에 데이터가 없으면 빈 구조체 인스턴스 반환
		return FWordsData();
	}

	// 랜덤한 인덱스를 생성하여 데이터 테이블의 행 중 하나를 선택
	int32 RandomIndex = FMath::RandRange(0, RowCount - 1);
	// 선택된 행의 이름
	FName SelectedRowName = RowNames[RandomIndex];

	// 선택된 행의 데이터를 가져옴
	FWordsData* SelectedRow = WordsDataTable->FindRow<FWordsData>(SelectedRowName, "");

	// 선택된 행의 데이터가 유효한지 확인
	if (SelectedRow)
	{
		//유효하다면 해당 데이터 반환
		return *SelectedRow;
	}

	return FWordsData();
}

void ARunningGameModeBase::MulticastSendQuizData_Implementation(const FWordsData& QuizData)
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PC = It->Get();
		if (PC)
		{
			// 플레이어 컨트롤러에서 위젯을 생성하고 데이터 전달
			ARunnerController* MyPC = Cast<ARunnerController>(PC);
			if (MyPC)
			{
				// 클라이언트가 자신의 로컬 플레이어 컨트롤러에서만 위젯을 생성하도록 명령
				MyPC->ClientCreateQuizWidget(QuizData);
				FString PCAddress = FString::Printf(TEXT("PlayerController Address: %p"), MyPC);
				PlayerCrossedFinishLine(MyPC);
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, PCAddress);
			}
	}
}}

