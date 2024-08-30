// Fill out your copyright notice in the Description page of Project Settings.


#include "JJH/Runner.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"
#include "Engine/Engine.h"

// Sets default values
ARunner::ARunner()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetRelativeLocation(FVector(0, 0, 40));

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom);

}

// Called when the game starts or when spawned
void ARunner::BeginPlay()
{
	Super::BeginPlay();

	// 1. 컨트롤러를 가져와서 PlayerController인지 캐스팅 해본다.
	auto* pc = Cast<APlayerController>(Controller);
	// 2. 캐스팅 성공했다면 
	if (pc)
	{
		// 3. 그 객체를 이용해서 EnhancedInputLocalPlayerSubsystem을 가져온다.
		UEnhancedInputLocalPlayerSubsystem* subSys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());
		// 4. SubSys를 이용해서 AddMappingContext를 한다.
		if (subSys)
		{
			subSys->AddMappingContext(IMC_Runner, 0);
		}
	}
}

// Called every frame
void ARunner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARunner::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInput)
	{
		EnhancedInput->BindAction(JJH_IA_Move, ETriggerEvent::Triggered, this, &ARunner::Run);
	}
}

void ARunner::Run(const FInputActionValue& value)
{
	if (HasAuthority())
	{
		if (GEngine && GetWorld())
		{
			// 화면에 디버깅 메시지 표시
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Server is handling the teleport request."));
		}
		// 서버에서 실행할 경우, 직접 이동을 처리합니다.
		ServerTeleportForward(RunningSpeed);

	}
	else
	{
		if (GEngine && GetWorld())
		{
			// 화면에 디버깅 메시지 표시
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Client is requesting teleport to server."));
		}
		// 클라이언트에서 서버 호출을 통해 이동을 요청합니다.
		ServerTeleportForward(RunningSpeed);
	}
}

void ARunner::ServerTeleportForward_Implementation(float Speed)
{
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	SetActorLocation(GetActorLocation() + Direction * Speed);

	MulticastTeleportForward(RunningSpeed);
}

void ARunner::MulticastTeleportForward_Implementation(float Speed)
{
	FVector Direction = FRotationMatrix(GetControlRotation()).GetScaledAxis(EAxis::X);
	SetActorLocation(GetActorLocation() + Direction * Speed);
}

