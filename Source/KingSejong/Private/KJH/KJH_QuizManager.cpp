// Fill out your copyright notice in the Description page of Project Settings.


#include "KJH/KJH_QuizManager.h"
#include "KJH/KJH_CommunityGameMode.h"
#include "KJH/KJH_PlayerController.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UKJH_QuizManager::UKJH_QuizManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;


}


// Called when the game starts
void UKJH_QuizManager::BeginPlay()
{
	Super::BeginPlay();

	// GameMode
	CommunityGameMode = Cast<AKJH_CommunityGameMode>(GetOwner());

	// OX 기준 찾기
 	TArray<AActor*> FoundActors;

	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("OXLine"), FoundActors);
	if (FoundActors.Num() > 0)
	{
		OXLine = FoundActors[0];
	}

	

	// StartQuiz();
}


// Called every frame
void UKJH_QuizManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UKJH_QuizManager::StartQuiz()
{
	if (CommunityGameMode == nullptr) return;
	if (OXLine == nullptr) return;

	CallBlockOxLineDelegate(false);

	//  플레이어 컨트롤러 리스트 갱신
	CommunityGameMode->GetAllPlayerControllers();
	if ( CommunityGameMode->PlayerControllers.Num() > 0 )
	{
		SetQuizState(EQuizState::Idle);
	}
}

void UKJH_QuizManager::CallBlockOxLineDelegate(bool bValue)
{
	if ( OnBlockOXLineDelegate.IsBound() )
		OnBlockOXLineDelegate.Broadcast(bValue);
}

void UKJH_QuizManager::SetQuizState(EQuizState State)
{
	 if (QuizState == State) return;

	QuizState = State;

	switch (QuizState)
	{
	case EQuizState::Idle:
		IdleState();
		break;
	case EQuizState::Question:
		QuestionState();
		break;
	case EQuizState::Waiting:
		WaitingState();
		break;
	case EQuizState::Answer:
		AnswerState();
		break;
	case EQuizState::End:
		EndState();
		break;
	default:
		break;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("QuizManager State: %s"), *UEnum::GetValueAsString(QuizState));
}

void UKJH_QuizManager::IdleState()
{
	// todo:  API로 문제 가져왔고, idletime이 지났으면 퀴즈 상태로 전환
	QuizCount = 0;

	// 모든 플레이어에게 문제, 정답 설정
	for ( auto pc : CommunityGameMode->PlayerControllers )
	{
		if ( pc == nullptr ) continue;

		pc->ClientRPC_InitializeQuiz(QuizTime);
	}

	FTimerHandle timerHandle;
	GetWorld()->GetTimerManager().SetTimer(timerHandle, FTimerDelegate::CreateLambda([this]()
		{
			SetQuizState(EQuizState::Question);
		}),
		IdleTime, false);
}

void UKJH_QuizManager::QuestionState()
{
	CallBlockOxLineDelegate(false);

	// 모든 플레이어에게 문제 설정
	for (auto pc : CommunityGameMode->PlayerControllers)
	{
		if (pc == nullptr) continue;
		if (QuizInfos.IsValidIndex(QuizCount) == false) continue;

		FString Question = QuizInfos[QuizCount].Question;

		pc->ClientRPC_ShowQuestionWidget(Question);
	}

	// 일정 시간이 지난 후 정답 공개 대기 상태로 전환
	FTimerHandle timerHandle;
	GetWorld()->GetTimerManager().SetTimer(timerHandle, FTimerDelegate::CreateLambda([this]()
		{
			SetQuizState(EQuizState::Waiting);
		}),
		QuizTime, false);
}

void UKJH_QuizManager::WaitingState()
{
	CallBlockOxLineDelegate(true);


	// 모든 플레이어에게 정답 대기 상태로 전환
	for (auto pc : CommunityGameMode->PlayerControllers)
	{
		if (pc == nullptr) continue;

		pc->ClientRPC_ShowWaitingWidget();
	}

	// 일정 시간 뒤 정답 공개 상태로 전환
	FTimerHandle timerHandle;
	GetWorld()->GetTimerManager().SetTimer(timerHandle, FTimerDelegate::CreateLambda([this]()
		{
			SetQuizState(EQuizState::Answer);

		}),
		WaitingTime, false);
}

void UKJH_QuizManager::AnswerState()
{
	// 퀴즈 정답 판별
	for (auto pc : CommunityGameMode->PlayerControllers)
	{
		if (pc == nullptr || pc->GetPawn() == nullptr) continue;

		// 실제 문제의 정답
		bool bCorrectAnswer = QuizInfos[QuizCount].Answer;
		// 플레이어가 선택한 정답
		bool bSelectedAnswer = GetPlayerQuizAnswer(pc);

		pc->ClientRPC_ShowWidgetAnswer(bCorrectAnswer, bSelectedAnswer);
	}

	// 퀴즈 카운드 증가
	QuizCount++;

	// 일정 시간이 지난 후
	FTimerHandle timerHandle;
	GetWorld()->GetTimerManager().SetTimer(timerHandle, FTimerDelegate::CreateLambda([this]()
		{
			//  풀 문제가 남았다면 다음 퀴즈로 전환
			if (QuizCount < QuizInfos.Num())
			{
				SetQuizState(EQuizState::Question);
			}
			// 모든 문제를 풀었다면 퀴즈 종료
			else
			{
				QuizCount = 0;
				SetQuizState(EQuizState::End);
			}
		}),
		AnwserTime, false);
}

void UKJH_QuizManager::EndState()
{
	CallBlockOxLineDelegate(false);
	

	// 모든 플레이어들에게 퀴즈 종료
	for (auto pc : CommunityGameMode->PlayerControllers)
	{
		pc->ClientRPC_EndQuiz();
	}

	OnEndQuizTimeDelegate.Broadcast();

	SetQuizState(EQuizState::NotStarted);
}

bool UKJH_QuizManager::GetPlayerQuizAnswer(AKJH_PlayerController* TargetPC)
{
	auto* target = TargetPC->GetPawn();

	FVector targetVector = target->GetActorLocation() - OXLine->GetActorLocation();
	FVector rightVector = OXLine->GetActorRightVector();

	float rightDot = FVector::DotProduct(targetVector, rightVector);

	// 오른쪽
	if (rightDot > 0)
		return false;
	// 왼쪽
	else
		return true;
}
