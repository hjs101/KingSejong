// Fill out your copyright notice in the Description page of Project Settings.


#include "JJH/RunningGameModeBase.h"
#include "JJH/RunningGameInstance.h"
#include "JJH/QuizWidget.h"
#include "JJH/RunnerController.h"
#include "JJH/Runner.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "JJH/JJH_GameInstance.h"

void ARunningGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	bUseSeamlessTravel = true;

	//FTimerHandle QuizTimer;
	////3초이따 퀴즈띄우기
	//GetWorld()->GetTimerManager().SetTimer(QuizTimer, this, &ARunningGameModeBase::StartQuiz, 3.f, false);

}

void ARunningGameModeBase::StartQuiz()
{
	//랜덤 데이터 가져오기
	SelectedQuiz = SelectRandomQuizData();

	//가져온데이터로 퀴즈뿌리기
	MulticastSendQuizData(SelectedQuiz);
}

void ARunningGameModeBase::PlayerCrossedFinishLine(ARunnerController* PlayerController)
{
	if (PlayerFinishOrder.Num() == 0) // 첫 번째 플레이어가 결승선에 도착했을 때
	{
		//n 초후에 다음 레벨로 이동, 로딩창 띄우기인데 지금은 n 초후에 정답 입력 UI 띄우기(= 위젯이랑 동기화애햐암)
		GetWorld()->GetTimerManager().SetTimer(ShowAnswerTextBoxTimerHandle, this, &ARunningGameModeBase::ShowAnswerUIToPlayerInOrder , CountDownNum, false);

		MulticastStartCountdownTimer();

		//모두의 위젯에서 카운트가 시작되어야함
	}
	//들어온 순서대로 더하기
	//들어오면 카운트하는 위젯 숨기기
	PlayerFinishOrder.Add(PlayerController);

	//들어온 사람들 승리 춤
	ARunner* Runner = Cast<ARunner>(PlayerController->GetPawn());
	if (Runner)
	{
		Runner->PlayWinMontage();
	}
	//MulticastShowLoading();

	PlayerController->ClientDisableInput();
	FailedToFinishPlayers.Remove(PlayerController);                                                                   
}

void ARunningGameModeBase::MoveToNextLevel()
{
	//아직은 세션이 없어서 알 수 없데용
	//FString NextLevelName = "ResultMap";  // 이동할 레벨의 이름
	//UGameplayStatics::OpenLevel(GetWorld() , FName(*NextLevelName) , false);
	//MulticastMoveToNextLevel();	
	
	if ( HasAuthority() )
	{
		FString NextLevelName = TEXT("/Game/JJH/ResultMap?listen");
		GetWorld()->ServerTravel(NextLevelName , true , false);
		
		//GEngine->AddOnScreenDebugMessage(-1 , 5.f , FColor::Red , TEXT("S"));
	}
	//GEngine->AddOnScreenDebugMessage(-1 , 5.f , FColor::Red , TEXT("u"));
}

void ARunningGameModeBase::MulticastShowLoading_Implementation()
{
	for ( ARunnerController* RPC : FailedToFinishPlayers )
	{
		RPC->ClientShowLoading();
	}	
	for ( ARunnerController* RPCc : PlayerFinishOrder )
	{
		RPCc->ClientShowLoading();
	}
	FTimerHandle TimerHandle;

	// 2초 후에 SubmitAnswer() 실행하는 코드
	GetWorld()->GetTimerManager().SetTimer(TimerHandle , [this]()
	{
			SubmitAnswer(); // 2초 후에 실행될 함수
	} , 2.0f , false); // 2.0f: 2초 후 실행, false: 반복하지 않음
}

void ARunningGameModeBase::MulticastStartCountdownTimer_Implementation()
{
	for (ARunnerController* RPC : FailedToFinishPlayers)
	{
		RPC->ClientStartWidgetCountDown();  // 클라이언트의 카운트다운 UI 시작
	}
}

void ARunningGameModeBase::ShowAnswerUIToPlayerInOrder()
{

	//들어온 사람들
// 	for (ARunnerController* RPC : PlayerFinishOrder)
// 	{
// 		RPC->ClientShowAnswerTextBox(); //정답박스 보여주기-> 이거는 다 들어오고 몇 초 이따가 보여줘야할듯?? 
// 	}
	//못 들어온 사람들
	for (ARunnerController* RPCS : FailedToFinishPlayers)
	{
		ARunner* Runner = Cast<ARunner>(RPCS->GetPawn());
		if (Runner)
		{
			//패배의 기어가기
			Runner->PlayLoseMontage();
		}
		//안 들어온 사람 입력 안받기
		RPCS->ClientDisableInput();
	}
	//3초 이따 로딩창
	FTimerHandle AnswerSubMitTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(AnswerSubMitTimerHandle , this , &ARunningGameModeBase::MulticastShowLoading , 3 , false);
}

void ARunningGameModeBase::AbleInput()
{
	for ( ARunnerController* RPCS : FailedToFinishPlayers )
	{
		//입력 안받기
		RPCS->ClientAbleInput();
	}
}
void ARunningGameModeBase::SubmitAnswer()
{
	if(PlayerFinishOrder.IsEmpty()) return;
	//첫번째 플레이어가 정답입력 찬스 가져감
	ARunnerController* CurrentController = *PlayerFinishOrder.begin();
	if ( CurrentController )
	{
		//정답입력차례인 플레이어
		CurrentController->ClientShowAnswerTextBox();

		for ( ARunnerController* OtherController : Players )
		{
			
			OtherController->ClientSpectatePlayer(CurrentController->GetPawn()); // 관전 모드 전환
			OtherController->ClientShowInitials();
		}	
	}
}

void ARunningGameModeBase::MoveToNextPlayer()
{
	ARunnerController* CurrentController = *PlayerFinishOrder.begin();
	//CurrentController->ClientHideLoading();
	CurrentController->ClientHideAnswerText();
	UE_LOG(LogTemp , Error , TEXT("%s") , *CurrentController->GetName());
	// 현재 차례의 플레이어를 배열에서 제거하고, 다음 차례로 넘김
	if ( PlayerFinishOrder.Num() > 0 )
	{
		PlayerFinishOrder.RemoveAt(0);  // 첫 번째 플레이어 제거
	}
	// 다음 플레이어에게 차례를 넘김
	SubmitAnswer();
}

//void ARunningGameModeBase::MuticastUpdateAnswerTextToAll_Implementation()
//{	
//
//}


void ARunningGameModeBase::UpdateTextInGameMode(const FString& AnswerText)
{
	for ( ARunnerController* PlayerController : Players )
	{
		if ( PlayerController )
		{
			PlayerController->ClientUpdateTextBoxContent(AnswerText);  // 모든 클라이언트의 텍스트박스를 업데이트
		}
	}
}

void ARunningGameModeBase::CheckAnswer(const FString& UserAnswer , ARunnerController* AnsweringPlayer)
{
	FString CorrectAnswer = SelectedQuiz.Word;
	if ( UserAnswer.Equals(CorrectAnswer) )
	{
		MulticastShowTeachSpeak(true);

	}
	else
	{
		MulticastShowTeachSpeak(false);
		FTimerHandle MovePlayerTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(MovePlayerTimerHandle, this, &ARunningGameModeBase::MoveToNextPlayer, 2.0f, false);
	}
}

void ARunningGameModeBase::EndGameAndReturnToLobby(ARunnerController* WinningPlayer)
{
	// 게임 인스턴스를 통해 세션 파괴
	UJJH_GameInstance* GameInstance = Cast<UJJH_GameInstance>(GetGameInstance());
	if ( GameInstance )
	{
		UE_LOG(LogTemp, Error, TEXT("ㅁㄴㅇㅁㄴㅇ1"));
		GameInstance->DestroySession();
	}
	UE_LOG(LogTemp, Error, TEXT("ㅁㄴㅇㅁㄴㅇ2"));
	// 모든 플레이어에게 게임 종료를 알림
	//MulticastEndGame(WinningPlayer);

	// 일정 시간 후 로비로 돌아가기(5초)
	FTimerHandle ReturnToLobbyTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(ReturnToLobbyTimerHandle, [ this ] ()
	{
		for ( FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It )
		{
			ARunnerController* PlayerController = Cast<ARunnerController>(It->Get());
			if ( PlayerController )
			{
				PlayerController->ClientReturnToLobby();
			}
		}
	}, 5.0f, false);  // 5초 후 로비로 돌아감
}


void ARunningGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	//게임모드에서 메시 바꿔주기
	//ARunner* MyCharacter = Cast<ARunner>(NewPlayer->GetPawn());
	//if ( MyCharacter )
	//{
	//	UJJH_GameInstance* GameInstance = Cast<UJJH_GameInstance>(GetGameInstance());
	//	if ( GameInstance && GameInstance->SelectedCharacterMesh )
	//	{
	//		MyCharacter->ServerSetCharacterMesh(GameInstance->SelectedCharacterMesh);
	//	}
	//}
	CurrentPlayerCount++;
	CheckAndStartQuiz();
}

void ARunningGameModeBase::CheckAndStartQuiz()
{
	if ( !bQuizStarted && CurrentPlayerCount >= RequiredPlayerCount )
	{
		bQuizStarted = true;

		FTimerHandle QuizTimer;
		GetWorld()->GetTimerManager().SetTimer(QuizTimer, this, &ARunningGameModeBase::StartQuiz, 3.f, false);
	}
}

void ARunningGameModeBase::MulticastShowTeachSpeak_Implementation(bool bIsCorrect)
{
	for ( ARunnerController* PlayerController : Players )
	{
		if ( PlayerController )
		{
			PlayerController->ClientShowTeacherSpeak(bIsCorrect);
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
	FTimerHandle InputHandle;
	GetWorld()->GetTimerManager().SetTimer(InputHandle , this , &ARunningGameModeBase::AbleInput , 3.f , false);

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PC = It->Get();
		if (PC)
		{
			// 플레이어 컨트롤러에서 위젯을 생성하고 데이터 전달
			ARunnerController* MyPC = Cast<ARunnerController>(PC);
			if (MyPC)
			{
				FailedToFinishPlayers.Add(MyPC);  // 배열에 추가
				Players.Add(MyPC);
				if (HasAuthority())
				{
					MyPC->ClientDisableInput();
					// 클라이언트가 자신의 로컬 플레이어 컨트롤러에서만 위젯을 생성하도록 명령
					MyPC->ClientCreateQuizWidget(QuizData);
					FString PCAddress = FString::Printf(TEXT("PlayerController Address: %p"), MyPC);
					//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, PCAddress);

					// OpenLevel 호출 후 레벨 이름 확인
					GetWorld()->GetTimerManager().SetTimerForNextTick([this]() {
						FString CurrentLevelAfterTravel = GetWorld()->GetMapName();
						FString ShortLevelAfterTravel = FPackageName::GetShortName(CurrentLevelAfterTravel);
						//GEngine->AddOnScreenDebugMessage(-1 , 5.f , FColor::Blue , FString::Printf(TEXT("Current Level After Travel: %s") , *ShortLevelAfterTravel));
						});
				}
			}
	}
}}

void ARunningGameModeBase::MulticastMoveToNextLevel_Implementation()
{
	FString NextLevelName = "ResultMap";  // 이동할 레벨의 이름
	GetWorld()->ServerTravel(TEXT("/Game/JJH/ResultMap.ResultMap?listen"));
}

