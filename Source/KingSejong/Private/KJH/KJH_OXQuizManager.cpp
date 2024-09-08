// Fill out your copyright notice in the Description page of Project Settings.


#include "KJH/KJH_OXQuizManager.h"
#include "KJH/KJH_CommunityGameModeBase.h"
#include "KJH/KJH_Player.h"

/*

게임모드에서 퀴즈 상태로 전환되면

플레이어의 위치가 퀴즈 위지로 변경된다.

플레이어들에게 퀴즈 위젯이 생성된다.

퀴즈 매니저는 웹통신으로 문제를 플레이어의 퀴즈 위젯에게 부여한다. ->델리게이트

퀴즈 위젯이 타이머에 의해 진행된다...


*/

// Sets default values
AKJH_OXQuizManager::AKJH_OXQuizManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AKJH_OXQuizManager::BeginPlay()
{
	Super::BeginPlay();
	
	// 게임 모드
	AGameModeBase* gameMode = GetWorld()->GetAuthGameMode();
	MyGameModeBase = Cast<AKJH_CommunityGameModeBase>(gameMode);

}

// Called every frame
void AKJH_OXQuizManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	CheckAnswer();

}

void AKJH_OXQuizManager::CheckAnswer()
{
	if(MyGameModeBase == nullptr) return;

	if ( HasAuthority() )
	{
		MyGameModeBase->GetAllPlayers();
		for ( auto player : MyGameModeBase->PlayerList )
		{
			if ( player == nullptr ) continue;

			EHorizontalState type = GetPlayerHorizontalState(player);

			if ( type == EHorizontalState::Left )
			{

			}
			else
			{

			}

			const FString myState = UEnum::GetValueAsString(type);
			DrawDebugString(GetWorld(), player->GetActorLocation(), myState, nullptr, FColor::Yellow, 0, true);
		}
	}

}

EHorizontalState AKJH_OXQuizManager::GetPlayerHorizontalState(AActor* Target)
{
	FVector targetVector = Target->GetActorLocation() - GetActorLocation();
	FVector rightVector = GetActorRightVector();

	float rightDot = FVector::DotProduct(targetVector, rightVector);

	if ( rightDot > 0 )
		return EHorizontalState::Right;
	else
		return EHorizontalState::Left;

}
