// Fill out your copyright notice in the Description page of Project Settings.


#include "HJS/TestActor.h"

// Sets default values
ATestActor::ATestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATestActor::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorldTimerManager().SetTimer(TestTimerHandle,this,&ATestActor::TimeTest,3.f,false);
	

}

// Called every frame
void ATestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATestActor::TimeTest()
{
	Test(GetWorld()->GetFirstPlayerController()->GetPawn());
}

void ATestActor::Test(AActor* OtherActor)
{
	SetOwner(OtherActor);
	ServerTest(OtherActor);
	UE_LOG(LogTemp, Warning, TEXT("111111111"));
}

void ATestActor::ServerTest_Implementation(AActor* OtherActor)
{
	SetOwner(OtherActor);
	UE_LOG(LogTemp,Warning,TEXT("0101010"));
}

void ATestActor::MulticastTest_Implementation(AActor* OtherActor)
{
}

