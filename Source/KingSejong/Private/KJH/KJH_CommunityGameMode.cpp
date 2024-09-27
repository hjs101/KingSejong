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
	// �÷��̾� ��Ʈ�ѷ� �迭 �ʱ�ȭ
	PlayerControllers.Empty();

	// ��� �÷��̾� ��Ʈ�ѷ��� ã�� �迭�� �߰�
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
//void AKJH_CommunityGameMode::SetCommunityState(ECommunityState NewState)
//{
//	//if (CommunityState == NewState) return;
//
//	// CommunityState = NewState;
//
//	// FTransform TargetTr = CommunityState == ECommunityState::Debate ? PlayerStartTr : QuizStartTr;
//
//	// (TargetTr);
//
//	// (LogTemp, Warning, TEXT("CommunityState : %d"), CommunityState);
//}

void AKJH_CommunityGameMode::PlayQuizMode()
{
	GetAllPlayerControllers();
}

void AKJH_CommunityGameMode::ExitSessionAllPlayers()
{
	GetAllPlayerControllers();

	for (auto pc : PlayerControllers)
	{
		pc->ClientRPC_ExitSession();
	}
}
