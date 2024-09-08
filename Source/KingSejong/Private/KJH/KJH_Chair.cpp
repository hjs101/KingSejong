// Fill out your copyright notice in the Description page of Project Settings.


#include "KJH/KJH_Chair.h"
#include "KJH/KJH_Player.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "Net/UnrealNetwork.h"

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

void AKJH_Chair::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AKJH_Chair, TargetPlayer);
}

void AKJH_Chair::OnBeginInteraction(AActor* OtherActor)
{
	Super::OnBeginInteraction(OtherActor);

	SetOwner(OtherActor);
	ServerSitDown(OtherActor);

}

void AKJH_Chair::OnEndInteraction()
{
	Super::OnEndInteraction();

	ServerStandUp();
}

bool AKJH_Chair::IsInteractable()
{
	return TargetPlayer == nullptr;
}

void AKJH_Chair::ServerSitDown_Implementation(AActor* OtherActor)
{
	
	DebugLog(TEXT("ServerSitDown_Implementation!!!!"));

	MulticastSitDown(OtherActor);
}


void AKJH_Chair::MulticastSitDown_Implementation(AActor* OtherActor)
{
	DebugLog(TEXT("MulticastSitDown_Implementation!!!"));

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
		TargetPlayer->OnEndSitDelegate.AddUObject(this, &AKJH_Chair::OnEndInteraction);

		FString msg = FString::Printf(TEXT("%s에 %d가 앉았습니다."), *this->GetName(), TargetPlayer->GetUniqueID());
		DebugLog(msg);
	}
	//UE_LOG(LogTemp, Warning, TEXT("%s에 %s가 앉았습니다."), *this->GetName(), *TargetPlayer->GetName());

	//SetOwner(OtherActor);
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

	TargetPlayer->OnEndSitDelegate.RemoveAll(this);

	FString msg = FString::Printf(TEXT("%s가 %s에서 일어났습니다."), *TargetPlayer->GetName(), *this->GetName());
	DebugLog(msg);

	TargetPlayer->OnEndSit();
    TargetPlayer = nullptr;

	SetOwner(nullptr);
}


void AKJH_Chair::DebugLog(FString Message)
{
	FString message;
	FColor color;
	if ( HasAuthority() )
	{
		message = FString::Printf(TEXT("Server : %s"), *Message);
		color = FColor::Cyan;
	}
	else
	{
		message = FString::Printf(TEXT("Client : %s"), *Message);
		color = FColor::Red;

	}

	// GEngine->AddOnScreenDebugMessage(-1, 100, color, message);
}