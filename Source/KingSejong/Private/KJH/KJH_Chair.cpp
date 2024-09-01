// Fill out your copyright notice in the Description page of Project Settings.


#include "KJH/KJH_Chair.h"
#include "KJH/KJH_Player.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"

AKJH_Chair::AKJH_Chair()
{

	bReplicates = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	SetRootComponent(BoxComp);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);

	SitArrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("SitArrowComp"));
	SitArrowComp->SetupAttachment(RootComponent);
	SitArrowComp->SetRelativeLocation(FVector(0, 0, 90));
}

void AKJH_Chair::BeginPlay()
{
	Super::BeginPlay();
}

void AKJH_Chair::OnBeginInteraction(AActor* OtherActor)
{
	Super::OnBeginInteraction(OtherActor);

	SitDown(OtherActor);
}

void AKJH_Chair::OnEndInteraction()
{
	Super::OnEndInteraction();

	StandUp();
}

bool AKJH_Chair::IsInteractable()
{
	return TargetPlayer == nullptr;
}

void AKJH_Chair::SitDown(AActor* OtherActor)
{
	DebugLog(TEXT("SitDown!!!"));

	SetOwner(OtherActor); // 먼저 소유자를 설정하고
	ServerSitDown(OtherActor); // 서버 RPC를 호출
}


void AKJH_Chair::ServerSitDown_Implementation(AActor* OtherActor)
{
	//UE_LOG(LogTemp, Warning, TEXT("ServerSitDown_Implementation!!!"));

	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Cyan, TEXT("ServerSitDown_Implementation!!!"));

	MulticastSitDown(OtherActor);
}


void AKJH_Chair::MulticastSitDown_Implementation(AActor* OtherActor)
{
	//UE_LOG(LogTemp, Warning, TEXT("MulticastSitDown_Implementation!!!"));
	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Cyan, TEXT("MulticastSitDown_Implementation!!!"));

	if ( TargetPlayer )
	{
		UE_LOG(LogTemp, Warning, TEXT("이미 앉아 있는 플레이어가 있습니다."));
		return;
	}

	TargetPlayer = Cast<AKJH_Player>(OtherActor);

	if ( TargetPlayer )
	{
		TargetPlayer->SetActorLocationAndRotation(SitArrowComp->GetComponentLocation(), SitArrowComp->GetComponentRotation());
		TargetPlayer->OnStartSit();
		TargetPlayer->OnEndSitDelegate.AddDynamic(this, &AKJH_Chair::OnEndInteraction);

		FString msg = FString::Printf(TEXT("%s에 %d가 앉았습니다."), *this->GetName(), TargetPlayer->GetUniqueID());
		DebugLog(msg);
	}
	//UE_LOG(LogTemp, Warning, TEXT("%s에 %s가 앉았습니다."), *this->GetName(), *TargetPlayer->GetName());

	SetOwner(OtherActor);
}

void AKJH_Chair::StandUp()
{
	ServerStandUp();
}

void AKJH_Chair::DebugLog(FString Message)
{
	FString message;
	if ( HasAuthority() )
	{
		
		message = FString::Printf(TEXT("Server : %s"), *Message);
	}
	else
	{
		message = FString::Printf(TEXT("Client : %s"), *Message);
	}

	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Cyan, message);
}

void AKJH_Chair::ServerStandUp_Implementation()
{
	MulticastStandUp();
}

void AKJH_Chair::MulticastStandUp_Implementation()
{
	if ( TargetPlayer == nullptr )
	{
		UE_LOG(LogTemp, Warning, TEXT("앉아 있는 플레이어가 없습니다."));
		return;
	}
	//UE_LOG(LogTemp, Warning, TEXT("%s가 %s에서 일어났습니다."), *TargetPlayer->GetName(), *this->GetName());

	
	FString msg = FString::Printf(TEXT("%s가 %s에서 일어났습니다."), *TargetPlayer->GetName(), *this->GetName());
	DebugLog(msg);

	TargetPlayer->OnEndSit();

	TargetPlayer = nullptr;

	SetOwner(nullptr);
}
