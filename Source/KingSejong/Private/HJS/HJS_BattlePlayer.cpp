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
#include "HJS/HJS_BattleGameMode.h"
#include "HJS/VoiceReceiver.h"
#include "Serialization/BufferArchive.h"
#include "Misc/Compression.h"
#include "HJS/GoogleNet.h"
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


	VoiceReceiver = CreateDefaultSubobject<UVoiceReceiver>(TEXT("VoiceReceiver"));

	GoogleNetComp = CreateDefaultSubobject<UGoogleNet>(TEXT("GoogleNetComp"));
}

// Called when the game starts or when spawned
void AHJS_BattlePlayer::BeginPlay()
{
	Super::BeginPlay();
	RecordFilePath = FPaths::Combine(FPaths::ProjectDir(), TEXT("RecordData/"));
	FString FileName = FString::Printf(TEXT("%s%dRecord"),*RecordFilePath, MyNum);
	
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
void AHJS_BattlePlayer::FirebaseLogin_Implementation()
{
	GoogleNetComp->AnonymousLogin();
}
// 1. 각각의 클라이언트에서 Wav 파일을 AI 서버에 전송 후, STT 결과를 내려받고, 그 결과를 Server에 전송
void AHJS_BattlePlayer::SendRecordToAIServer()
{
	STTResult = "Hello Unreal";
	SendSTTResultToGameMode(STTResult);
}

void AHJS_BattlePlayer::SendSTTResultToGameMode_Implementation(const FString& Result)
{
	// 서버로 만들었지만 방어코드, 서버인지 확인
	if (HasAuthority())
	{
		// GM을 가져와서 저장하기
		AHJS_BattleGameMode* GM = Cast<AHJS_BattleGameMode>(GetWorld()->GetAuthGameMode());
		
		APlayerController* PC = Cast<APlayerController>(GetController());
		if (!PC)
		{
			return;
		}
		GM->SettingPlayerAnswer(Result, PC);

	}
}

void AHJS_BattlePlayer::SendWavToServer_Implementation()
{
	TArray<uint8> OutByteArray;
	FString FileName = RecordFileName + TEXT(".wav");
	FString FilePath = RecordFilePath + FileName;

	GoogleNetComp->FileUploadToFirebase(FilePath,FileName);

}

void AHJS_BattlePlayer::ReceiveServerSoundData_Implementation(const FString& FileName)
{
	if (HasAuthority())
	{	
		
	}
}

void AHJS_BattlePlayer::ClientPlaySoundData(const TArray<uint8>& SoundData)
{
	USoundWaveProcedural* SoundWave = NewObject<USoundWaveProcedural>();
	SoundWave->SetSampleRate(44100); // 샘플 레이트 설정
	SoundWave->NumChannels = 1; // 채널 수 설정 (모노)
	SoundWave->Duration = 5.0f; // 재생 시간 설정
	SoundWave->QueueAudio(SoundData.GetData(), SoundData.Num());

	UAudioComponent* AudioComponent = UGameplayStatics::SpawnSound2D(GetWorld(), SoundWave);
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

void AHJS_BattlePlayer::StartRecording_Implementation(const FString& PlayerID)
{
	if (GetWorld()->GetAuthGameMode() == nullptr)
	{
		UE_LOG(LogTemp,Warning,TEXT("GM is NULL!"));
	}
	FirebaseLogin();
	if (PlayerRecordID == "")
	{
		PlayerRecordID = PlayerID;
	}

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
		RecordFileName = FString::Printf(TEXT("%s_Record"), *PlayerRecordID);
		UAudioMixerBlueprintLibrary::StopRecordingOutput(GetWorld(), EAudioRecordingExportType::WavFile, RecordFileName, RecordFilePath, RecordSound);
		SendRecordToAIServer();
	}
}

void AHJS_BattlePlayer::PlayerHit()
{
	
}

void AHJS_BattlePlayer::ServerPlayerHit_Implementation()
{
}

void AHJS_BattlePlayer::MulticastPlayerHit_Implementation()
{
}
