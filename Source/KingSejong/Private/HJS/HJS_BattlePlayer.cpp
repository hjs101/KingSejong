// Fill out your copyright notice in the Description page of Project Settings.


#include "HJS/HJS_BattlePlayer.h"
#include "Kismet/GameplayStatics.h"
#include "AudioCaptureComponent.h"
#include "AudioDevice.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Sound/SoundWaveProcedural.h"
#include "AudioMixerBlueprintLibrary.h"
#include "HJS/BattlePlayerAnim.h"

// Sets default values
AHJS_BattlePlayer::AHJS_BattlePlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	// 오디오 캡처 컴포넌트 초기화
	AudioCapture = CreateDefaultSubobject<UAudioCaptureComponent>(TEXT("AudioCapture"));
	AudioCapture->bAutoActivate = false; // 자동으로 시작되지 않도록 설정

}

// Called when the game starts or when spawned
void AHJS_BattlePlayer::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AHJS_BattlePlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AHJS_BattlePlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AHJS_BattlePlayer::Attack()
{
	ServerAttack();
}

void AHJS_BattlePlayer::Punch()
{
	UBattlePlayerAnim* Anim = Cast<UBattlePlayerAnim>(GetMesh()->GetAnimInstance());
	if (Anim)
	{
		Anim->JumpMontageToAttack();
	}
}

void AHJS_BattlePlayer::ServerAttack_Implementation()
{
	MulticastAttack();
}

void AHJS_BattlePlayer::MulticastAttack_Implementation()
{
	UBattlePlayerAnim* Anim = Cast<UBattlePlayerAnim>(GetMesh()->GetAnimInstance());
	if (Anim)
	{
		Anim->PlayChargingMontage();
	}

	GetWorldTimerManager().SetTimer(PunchHandle, this, &AHJS_BattlePlayer::Punch, 3.f, false);
}

void AHJS_BattlePlayer::StartRecording_Implementation(int32 PlayerNum)
{
	MyNum = PlayerNum;
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TestVFX, GetActorTransform());
	if (!bIsRecording)
	{
		bIsRecording = true;
		AudioCapture->Start();  // 오디오 캡처 시작
		UAudioMixerBlueprintLibrary::StartRecordingOutput(GetWorld(), 0.f, RecordSound);
		GetWorldTimerManager().SetTimer(RecordHandle, this, &AHJS_BattlePlayer::StopRecording, 5.f, false);
	}
}

void AHJS_BattlePlayer::StopRecording_Implementation()
{
	if (bIsRecording)
	{
		bIsRecording = false;
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TestVFX, GetActorTransform());
		AudioCapture->Stop();  // 오디오 캡처 중지
		FString FilePath = FPaths::Combine(FPaths::ProjectSavedDir(), TEXT("HJS/Audio/"));
		FString FileName = FString::Printf(TEXT("%dRecord"), MyNum);
		UAudioMixerBlueprintLibrary::StopRecordingOutput(GetWorld(), EAudioRecordingExportType::WavFile, FileName, TEXT("HJS/Audio/"), RecordSound);
		Attack();
	}
}
