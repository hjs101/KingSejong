// Fill out your copyright notice in the Description page of Project Settings.


#include "KJH/KJH_PlayerInteraction.h"
#include "KJH/KJH_Player.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Blueprint/UserWidget.h"
#include "KJH/KJH_InteractiveActor.h"
#include "KJH/KJH_PlayerController.h"


// Sets default values for this component's properties
UKJH_PlayerInteraction::UKJH_PlayerInteraction()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;

	//SetIsReplicatedByDefault(true);
}

void UKJH_PlayerInteraction::InitializeComponent()
{
	Super::InitializeComponent();
	
	MyActor = CastChecked<AKJH_Player>(GetOwner());
	if (MyActor)
	{
		MyActor->OnInputBindingDelegate.AddUObject(this, &UKJH_PlayerInteraction::SetupInputBinding);
	}

	//MyPlayerController = Cast<AKJH_PlayerController>(GetWorld()->GetFirstPlayerController());

}

// Called when the game starts
void UKJH_PlayerInteraction::BeginPlay()
{
	Super::BeginPlay();

	if ( MyActor && MyActor->IsLocallyControlled() )
		CreateKeyGuide();
}


// Called every frame
void UKJH_PlayerInteraction::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!MyActor || !MyActor->CameraComp) return;

	FVector start = MyActor->CameraComp->GetComponentLocation();
	FVector end = start + MyActor->CameraComp->GetForwardVector() * TraceDistance;

	FHitResult outHit;
	ECollisionChannel traceChannel = ECC_Visibility;
	FCollisionQueryParams params;
	params.AddIgnoredActor(MyActor);

	// 인터렉션 대상 찾기
	HitActor = nullptr;

	if (GetWorld()->LineTraceSingleByChannel(outHit, start, end, traceChannel, params))
	{
		AActor* actor = outHit.GetActor();
		if (actor != nullptr)
		{
            HitActor = Cast<AKJH_InteractiveActor>(actor);

            if (HitActor)
            {
                //GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, outHit.GetActor()->GetName());
            }
		}
	}
	else
	{
		//DrawDebugLine(GetWorld(), start, end, FColor::Cyan, false, 1);
	}

	SetActiveKeyGuide(IsInteractableActor(HitActor));
}

void UKJH_PlayerInteraction::SetupInputBinding(UEnhancedInputComponent* Input)
{
	Input->BindAction(IA_Interaction, ETriggerEvent::Started, this, &UKJH_PlayerInteraction::OnActionInteraction);

	UE_LOG(LogTemp, Warning, TEXT("SetupInputBinding Call!!"))
}

void UKJH_PlayerInteraction::OnActionInteraction(const FInputActionValue& value)
{
	// HitActor 상호작용
	if(HitActor == nullptr) return;
	if(HitActor->IsInteractable() == false) return;



	//ServerRPC_InteractiveActor(HitActor, MyPlayerController);
	ServerRPCInteractiveActor(HitActor, MyActor);


	UE_LOG(LogTemp, Warning, TEXT("OnActionInteraction!!"));
}


void UKJH_PlayerInteraction::CreateKeyGuide()
{
	if ( KeyGuideWidget ) return;
	// UI
	KeyGuideWidget = CreateWidget(GetWorld(), KeyGuideFactory);
	if ( KeyGuideWidget )
	{
		KeyGuideWidget->AddToViewport();
		SetActiveKeyGuide(false);
	}
}

void UKJH_PlayerInteraction::SetActiveKeyGuide(bool bValue)
{
	if ( KeyGuideWidget == nullptr ) return;
	if (bValue)
	{
		KeyGuideWidget->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		KeyGuideWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

bool UKJH_PlayerInteraction::IsInteractableActor(AKJH_InteractiveActor* OtherActor)
{
	return OtherActor && OtherActor->IsInteractable();
}

void UKJH_PlayerInteraction::ServerRPCInteractiveActor_Implementation(AKJH_InteractiveActor* TargetActor, AKJH_Player* PlayerActor)
{
	TargetActor->SetOwner(PlayerActor);
	TargetActor->OnBeginInteraction(PlayerActor);
}

void UKJH_PlayerInteraction::ServerRPC_InteractiveActor_Implementation(AKJH_InteractiveActor* TargetActor, AKJH_PlayerController* PC)
{
	TargetActor->SetOwner(PC);
	TargetActor->OnBeginInteraction(PC->GetPawn());
}