// Fill out your copyright notice in the Description page of Project Settings.


#include "KJH/KJH_QuizManager.h"
#include "KJH/KJH_CommunityGameModeBase.h"
#include "KJH/KJH_Player.h"


// Sets default values
AKJH_QuizManager::AKJH_QuizManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AKJH_QuizManager::BeginPlay()
{
	Super::BeginPlay();
	
	// 게임 모드
	AGameModeBase* gameMode = GetWorld()->GetAuthGameMode();
	MyGameModeBase = Cast<AKJH_CommunityGameModeBase>(gameMode);

}

// Called every frame
void AKJH_QuizManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	CheckAnswer();

}

void AKJH_QuizManager::CheckAnswer()
{
	if(MyGameModeBase == nullptr) return;


	MyGameModeBase->GetAllPlayers();
	for ( auto player : MyGameModeBase->PlayerList )
	{
		if(player == nullptr) continue;

		EHorizontalType type = GetPlayerHorizontalType(player);

		if ( type == EHorizontalType::Left )
		{

		}
		else
		{

		}

		const FString myState = UEnum::GetValueAsString(type);
		DrawDebugString(GetWorld(), player->GetActorLocation(), myState, nullptr, FColor::Yellow, 0, true);
	}

}

EHorizontalType AKJH_QuizManager::GetPlayerHorizontalType(AActor* Target)
{
	FVector targetVector = Target->GetActorLocation() - GetActorLocation();
	FVector rightVector = GetActorRightVector();

	float rightDot = FVector::DotProduct(targetVector, rightVector);

	if ( rightDot > 0 )
		return EHorizontalType::Right;
	else
		return EHorizontalType::Left;

}
