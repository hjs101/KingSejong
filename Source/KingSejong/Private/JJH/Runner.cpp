// Fill out your copyright notice in the Description page of Project Settings.


#include "JJH/Runner.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"
#include "Engine/Engine.h"
#include "JJH/JJH_GameInstance.h"
#include "Net/UnrealNetwork.h"

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

	//UJJH_GameInstance* GI = Cast<UJJH_GameInstance>(GetGameInstance());
	//APlayerController* PC = Cast<APlayerController>(GetController());
	//if ( GI && PC )
	//{
	//	USkeletalMesh** FoundMesh = GI->PlayerMeshes.Find(PC);
	//	if ( FoundMesh && *FoundMesh )
	//	{
	//		UpdateMesh(*FoundMesh);
	//	}
	//}
	//else
	//{
	//	// 클라이언트에서 서버에 메시 정보 요청
	//	GI->ServerSetPlayerMesh(PC, nullptr);
	//}
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

void ARunner::PlayRunMontageSection(const FName& SectionName)
{
	if (RunMontage && GetMesh()->GetAnimInstance())
	{
		GetMesh()->GetAnimInstance()->Montage_Play(RunMontage);
		GetMesh()->GetAnimInstance()->Montage_JumpToSection(SectionName, RunMontage);
	}
}
void ARunner::PlayWinMontage()
{
	if (HasAuthority())
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("playwin")));
		MulticastPlayWinMontage();
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("playwinelse")));
		ServerPlayWinMontage();
	}
}
void ARunner::ServerPlayWinMontage_Implementation()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("serveerplaywin")));
	MulticastPlayWinMontage();
}

void ARunner::MulticastPlayWinMontage_Implementation()
{
	if ( WinMontage )
	{
		//GetMesh()->GetAnimInstance()->Montage_Stop(0,RunMontage);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Montage is playing"));
		//PlayAnimMontage(WinMontage);
		GetMesh()->GetAnimInstance()->Montage_Play(WinMontage);
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("WinMontage is nullptr"));
	}
	bCanDance = true;
}

void ARunner::PlayLoseMontage()
{
	if (HasAuthority())
	{
		ServerPlayLoseMontage();
	}
}
void ARunner::ServerPlayLoseMontage_Implementation()
{
	PlayAnimMontage(LoseMontage);

	MulticastPlayLoseMontage();
}
void ARunner::MulticastPlayLoseMontage_Implementation()
{
	PlayAnimMontage(LoseMontage);
}


void ARunner::Run(const FInputActionValue& value)
{

	if (HasAuthority())
	{
		// 서버에서 실행할 경우, 직접 이동을 처리합니다.
		ServerTeleportForward(RunningSpeed, value.Get<float>());
	}
	else
	{
		// 클라이언트에서 서버 호출을 통해 이동을 요청합니다.
		ServerTeleportForward(RunningSpeed, value.Get<float>());
	}
}

void ARunner::ServerTeleportForward_Implementation(float Speed, float InputValue)
{

	FName SectionToPlay;

	if (InputValue < 0)  // 'A' 키 입력
	{
		SectionToPlay = "Left";
	}
	else if (InputValue > 0)  // 'D' 키 입력
	{
		SectionToPlay = "Right";
	}

	if (!SectionToPlay.IsNone())
	{
		PlayRunMontageSection(SectionToPlay);
	}
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	SetActorLocation(GetActorLocation() + Direction * Speed);

	MulticastTeleportForward(RunningSpeed, InputValue);
} 

void ARunner::MulticastTeleportForward_Implementation(float Speed, float InputValue)
{
	FName SectionToPlay;

	if (InputValue < 0)  // 'A' 키 입력
	{
		SectionToPlay = "Left";
	}
	else if (InputValue > 0)  // 'D' 키 입력
	{
		SectionToPlay = "Right";
	}

	if (!SectionToPlay.IsNone())
	{
		PlayRunMontageSection(SectionToPlay);
	}

	FVector Direction = FRotationMatrix(GetControlRotation()).GetScaledAxis(EAxis::X);
	SetActorLocation(GetActorLocation() + Direction * Speed);
}


//====================================


//void ARunner::UpdateCharacterMesh()
//{
//	UJJH_GameInstance* GameInstance = Cast<UJJH_GameInstance>(GetGameInstance());
//	if ( GameInstance && GameInstance->SelectedCharacterMesh )
//	{
//		GetMesh()->SetSkeletalMesh(GameInstance->SelectedCharacterMesh);
//	}
//}
//
//void ARunner::OnRep_CharacterMesh()
//{
//	if ( CharacterMesh )
//	{
//		GetMesh()->SetSkeletalMesh(CharacterMesh);
//	}
//}

void ARunner::UpdateMesh(USkeletalMesh* NewMesh)
{
	if ( NewMesh )
	{
		GetMesh()->SetSkeletalMesh(NewMesh);
		UE_LOG(LogTemp, Error, TEXT("qwer5"));
	}
}

//void ARunner::ServerSetCharacterMesh_Implementation(USkeletalMesh* NewMesh)
//{
//	CharacterMesh = NewMesh;
//	OnRep_CharacterMesh();
//}
//
//void ARunner::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
//{
//	//캐릭터 메시 리플리케이트
//	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//	DOREPLIFETIME(ARunner, CharacterMesh);
//}


//인터페이스 상속
//게임모드 포스트 로그인에서 GI->SyncAllPlayerMeshes();
//