// Fill out your copyright notice in the Description page of Project Settings.


#include "KJH/KJH_Player.h"
#include "KJH/KJH_PlayerInteraction.h"
#include "KJH/KJH_PlayerAnimInstance.h"
#include "KJH/KJH_VoiceRecorder.h"
#include "KJH/API/KJH_HttpManager.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "KJH/KJH_CommunityGameMode.h"

// Sets default values
AKJH_Player::AKJH_Player()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 200.0;
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetRelativeLocation(FVector(0, 0, 60));

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));

	InteractionComp = CreateDefaultSubobject<UKJH_PlayerInteraction>(TEXT("InteractionComp"));
	VoiceRecorderComp = CreateDefaultSubobject<UKJH_VoiceRecorder>(TEXT("VoiceRecorderComp"));

	MoveAudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComp"));
	MoveAudioComp->SetupAttachment(RootComponent);
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
	
	// GameMode
	if (HasAuthority())
	{
		MyGameMode = Cast<AKJH_CommunityGameMode>(GetWorld()->GetAuthGameMode());
	}

}

// Called every frame
void AKJH_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AKJH_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	if (input)
	{
		// Move
		input->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AKJH_Player::OnActionMove);
        input->BindAction(IA_Move, ETriggerEvent::Completed, this, &AKJH_Player::OnActionMoveStop);

		// Lock
		input->BindAction(IA_Look, ETriggerEvent::Triggered, this, &AKJH_Player::OnActionLook);

		// Jump
		input->BindAction(IA_Jump, ETriggerEvent::Started, this, &AKJH_Player::OnActionJump);
	
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

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	AddMovementInput(ForwardDirection, v.Y);
	AddMovementInput(RightDirection, v.X);


	if (MoveAudioComp->IsPlaying() == false || GetCharacterMovement()->IsFalling())
	{
		ServerRPC_TogglePlayMoveSound(true);
		// bIsMove = true;
	}

}


void AKJH_Player::OnActionMoveStop(const FInputActionValue& value)
{
	//if (bIsSit) return;

	ServerRPC_TogglePlayMoveSound(false);

	//bIsMove = false;
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

	ServerRPC_TogglePlayMoveSound(false);
	ServerRPC_PlayJumpSound();

	if ( OnEndSitDelegate.IsBound() )
	{
		OnEndSitDelegate.Broadcast();
		// OnEndSit();
		UE_LOG(LogTemp, Warning, TEXT("OnEndSitDelegate!!"));
	}
	
}

void AKJH_Player::SetIsSit(bool bValue)
{
	bIsSit = bValue;
}

void AKJH_Player::OnStartSit(FVector TargetLoc, FRotator TargetRot)
{
	if (PlayerAnim == nullptr) return;
	if (bIsSit) return;

	GetCharacterMovement()->bOrientRotationToMovement = false;
	SetActorLocationAndRotation(TargetLoc, TargetRot);

	SetIsSit(true);
}

void AKJH_Player::OnEndSit()
{
	if (PlayerAnim == nullptr) return;
	if (bIsSit == false) return;

	GetCharacterMovement()->bOrientRotationToMovement = true;

	SetIsSit(false);
}

void AKJH_Player::SetPlayerPosition(FTransform TargetTransform)
{
	bIsSit = false;

	// todo: voice 녹음 중이라면 정지?
	// UI도 다 닫혀야 함
	SetActorLocationAndRotation(TargetTransform.GetLocation(), TargetTransform.GetRotation());

}

void AKJH_Player::ServerRPC_TogglePlayMoveSound_Implementation(bool bValue)
{
	if(MyGameMode == nullptr) return;

    USoundBase* footSound = MyGameMode->CommunityMode == ECommunityMode::Community ? SFX_Move_Wood : SFX_Move_Dirt;


    MulticastRPC_TogglePlayMoveSound(bValue, footSound);
}

void AKJH_Player::MulticastRPC_TogglePlayMoveSound_Implementation(bool bValue, class USoundBase* Sound)
{
	if (bValue)
	{
		MoveAudioComp->SetSound(Sound);
		MoveAudioComp->Play();
	}
	else
	{

		MoveAudioComp->Stop();
	}
}

void AKJH_Player::ServerRPC_PlayJumpSound_Implementation()
{
	MulticastRPC_PlayJumpSound();

}

void AKJH_Player::MulticastRPC_PlayJumpSound_Implementation()
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), SFX_Jump, GetActorLocation(), 1, 1, 0, SA_Player);
}
