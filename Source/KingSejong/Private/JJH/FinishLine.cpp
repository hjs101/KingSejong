// Fill out your copyright notice in the Description page of Project Settings.


#include "JJH/FinishLine.h"
#include "Components/BoxComponent.h"
#include "JJH/RunningGameModeBase.h"
#include "JJH/RunnerController.h"
#include "JJH/Runner.h"

// Sets default values
AFinishLine::AFinishLine()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// OverlapBox 컴포넌트 생성 및 설정
	OverlapBox = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapBox"));
	OverlapBox->SetupAttachment(RootComponent);
	OverlapBox->SetCollisionProfileName(TEXT("Trigger"));
	OverlapBox->OnComponentBeginOverlap.AddDynamic(this, &AFinishLine::OnOverlapBegin);
}


void AFinishLine::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Overlapped"));
    // 플레이어가 오버랩 되었는지 확인
    if (OtherActor && OtherActor != this)
    {
        APlayerController* PlayerController = Cast<APlayerController>(OtherActor->GetInstigatorController());
        if (PlayerController)
        {
            // 서버에서만 작동
            if (HasAuthority())
            {
                // 게임 모드에 알리기
                ARunningGameModeBase* GameMode = Cast<ARunningGameModeBase>(GetWorld()->GetAuthGameMode());
                if (GameMode)
                {
                    GameMode->PlayerCrossedFinishLine(Cast<ARunnerController>(PlayerController));
                }
            }
        }
    }
}

// Called when the game starts or when spawned
void AFinishLine::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFinishLine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

