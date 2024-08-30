// Fill out your copyright notice in the Description page of Project Settings.


#include "KJH/KJH_PlayerInteraction.h"
#include "KJH/KJH_Player.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Blueprint/UserWidget.h"
#include "KJH/KJH_InteractiveActor.h"

// Sets default values for this component's properties
UKJH_PlayerInteraction::UKJH_PlayerInteraction()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
}

void UKJH_PlayerInteraction::InitializeComponent()
{
	Super::InitializeComponent();
	
	MyActor = CastChecked<AKJH_Player>(GetOwner());
	if (MyActor)
	{
		MyActor->OnInputBindingDelegate.AddUObject(this, &UKJH_PlayerInteraction::SetupInputBinding);
		UE_LOG(LogTemp, Warning, TEXT("SetupInputBinding..."));
	}

}

// Called when the game starts
void UKJH_PlayerInteraction::BeginPlay()
{
	Super::BeginPlay();

	// UI
	KeyGuideWidget = CreateWidget(GetWorld(), KeyGuideFactory);
	if (KeyGuideWidget)
	{
		KeyGuideWidget->AddToViewport();
		SetActiveKeyGuide(false);
	}
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
            else
            {
                HitActor = nullptr;
            }
		}

	}
	else
	{
		//DrawDebugLine(GetWorld(), start, end, FColor::Cyan, false, 1);
	}
	SetActiveKeyGuide(HitActor != nullptr);
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

	HitActor->OnInteracted();

	UE_LOG(LogTemp, Warning, TEXT("OnActionInteraction!!"));
}


void UKJH_PlayerInteraction::SetActiveKeyGuide(bool bValue)
{
	if (bValue)
	{
		KeyGuideWidget->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		KeyGuideWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}
