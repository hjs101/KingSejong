// Fill out your copyright notice in the Description page of Project Settings.


#include "KJH/KJH_CommunityGameMode.h"
#include "KJH/KJH_PlayerController.h"
#include "Kismet/GameplayStatics.h"

void AKJH_CommunityGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void AKJH_CommunityGameMode::GetAllPlayerControllers()
{
	// 플레이어 컨트롤러 배열 초기화
	PlayerControllers.Empty();

	// 모든 플레이어 컨트롤러를 찾아 배열에 추가
	int32 playerCount = GetWorld()->GetNumPlayerControllers();

	for (int i = 0; i < playerCount; i++)
	{
		auto* ps = Cast<AKJH_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), i));

		if (ps)
		{
			PlayerControllers.Add(ps);
		}
	}
}
void AKJH_CommunityGameMode::SetCommunityState(ECommunityState NewState)
{
	//if (CommunityState == NewState) return;

	// CommunityState = NewState;

	// FTransform TargetTr = CommunityState == ECommunityState::Debate ? PlayerStartTr : QuizStartTr;

	// (TargetTr);

	// (LogTemp, Warning, TEXT("CommunityState : %d"), CommunityState);
}

void AKJH_CommunityGameMode::PlayQuizMode()
{
	GetAllPlayerControllers();
}
