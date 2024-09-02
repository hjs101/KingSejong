// Fill out your copyright notice in the Description page of Project Settings.


#include "KJH/KJH_Player.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "KJH/KJH_PlayerInteraction.h"
#include "KJH/KJH_PlayerAnimInstance.h"
#include "Blueprint/UserWidget.h"
#include "Net/UnrealNetwork.h"


// Sets default values
AKJH_Player::AKJH_Player()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	InteractionComp = CreateDefaultSubobject<UKJH_PlayerInteraction>(TEXT("InteractionComp"));

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 200.0;
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetRelativeLocation(FVector(0, 0, 60));

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
}

// Called when the game starts or when spawned
void AKJH_Player::BeginPlay()
{
	Super::BeginPlay();

	// Input
	APlayerController* pc = CastChecked<APlayerController>(GetWorld()->GetFirstPlayerController());
	if (pc)
	{
		auto* subSys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());
		if (subSys)
		{
			subSys->AddMappingContext(IMC_Player, 0);
		}
	}

	// Animation
	PlayerAnim = CastChecked<UKJH_PlayerAnimInstance>(GetMesh()->GetAnimInstance());

}

// Called every frame
void AKJH_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//FTransform t = FTransform(GetControlRotation());
	//Direction = t.TransformVector(Direction);
	//Direction.Z = 0;

	//AddMovementInput(Direction);
	//Direction = FVector::ZeroVector;
}

// Called to bind functionality to input
void AKJH_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	if (input)
	{
		input->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AKJH_Player::OnActionMove);
		input->BindAction(IA_Look, ETriggerEvent::Triggered, this, &AKJH_Player::OnActionLook);
		input->BindAction(IA_Jump, ETriggerEvent::Started, this, &AKJH_Player::OnActionJump);
		// input->BindAction(IA_Interaction, ETriggerEvent::Started, this, &AKJH_Player::OnActionInteraction);
	
		OnInputBindingDelegate.Broadcast(input);

		UE_LOG(LogTemp, Warning, TEXT("OnInputBindingDelegate Call!!"));
	}
}

void AKJH_Player::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AKJH_Player, bIsSit);
}

void AKJH_Player::OnActionMove(const FInputActionValue& value)
{
	// 앉아있는 상태라면 움직임 종료
	if (bIsSit) return;
		
	FVector2D v = value.Get<FVector2D>();

	//Direction.X = v.X;
	//Direction.Y = v.Y;

	// find out which way is forward
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get forward vector
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// get right vector 
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	// add movement 
	AddMovementInput(ForwardDirection, v.Y);
	AddMovementInput(RightDirection, v.X);


}

void AKJH_Player::OnActionLook(const FInputActionValue& value)
{
	FVector2D v = value.Get<FVector2D>();

	AddControllerPitchInput(-v.Y);
	AddControllerYawInput(v.X);
}

void AKJH_Player::OnActionJump(const FInputActionValue& value)
{
	Jump();

	OnEndSitDelegate.Broadcast();
	//OnEndSit();
}

void AKJH_Player::SetIsSit(bool bValue)
{
	//if ( HasAuthority() )
	//{
		bIsSit = bValue;
	//}
}

void AKJH_Player::OnStartSit()
{
	if (PlayerAnim == nullptr) return;
	if (bIsSit) return;

	//PlayerAnim->SetSitState(true);
	SetIsSit(true);

}

void AKJH_Player::OnEndSit()
{
	if (PlayerAnim == nullptr) return;
	if (bIsSit == false) return;

	
	//PlayerAnim->SetSitState(false);
	//bIsSit = false;
	SetIsSit(false);
}
