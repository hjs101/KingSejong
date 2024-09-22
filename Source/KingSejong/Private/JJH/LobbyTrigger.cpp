// Fill out your copyright notice in the Description page of Project Settings.


#include "JJH/LobbyTrigger.h"
#include "Components/BoxComponent.h"
#include "JJH/JJH_GameInstance.h"
#include "JJH/LobbyWidget.h"

// Sets default values
ALobbyTrigger::ALobbyTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
		
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	BoxCollision->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ALobbyTrigger::BeginPlay()
{
	Super::BeginPlay();
	
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ALobbyTrigger::ComponentBeginOverlap);
}

// Called every frame
void ALobbyTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALobbyTrigger::ComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UJJH_GameInstance* gi = Cast<UJJH_GameInstance>(GetGameInstance());
	
	ULobbyWidget* Lobby = CreateWidget<ULobbyWidget>(GetWorld(), LobbyWidgetFactory);
	Lobby->SetMenuInterface(gi);
	Lobby->SetTextAndCategory(TriggerText, TriggerCategory);
	Lobby->AddToViewport();

}

