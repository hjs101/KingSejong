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
#include "Net/UnrealNetwork.h"
#include "HJS/MainUI.h"
#include "HJS/BattleQuestionStruct.h"
#include "HJS/AINet.h"
#include "../../../../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraComponent.h"
#include "../../../../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraSystemInstance.h"
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

	AINetComp = CreateDefaultSubobject<UAINet>(TEXT("AINetComp"));

	ChargingRightHandComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ChargingRightHandComp"));
	ChargingRightHandComp->SetupAttachment(GetMesh(), TEXT("middle_01_r"));
	ChargingRightHandComp->SetAutoActivate(true);  // 기본적으로 비활성화

	ChargingLeftHandComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ChargingLeftHandComp"));
	ChargingLeftHandComp->SetupAttachment(GetMesh(), TEXT("middle_01_l"));
	ChargingLeftHandComp->SetAutoActivate(true);  // 기본적으로 활성화
}

// Called when the game starts or when spawned
void AHJS_BattlePlayer::BeginPlay()
{
	Super::BeginPlay();
	RecordFilePath = FPaths::Combine(FPaths::ProjectDir(), TEXT("RecordData/"));
	GoogleNetComp->Me = this;
	AINetComp->Me = this;
	if (HasAuthority())
	{
		GetWorldTimerManager().SetTimer(JoinTimerHandle, this, &AHJS_BattlePlayer::LoginSignal, 0.05f, false);
	}
	if (!IsLocallyControlled())
	{
		GetMesh()->SetRenderCustomDepth(true);
	}
}

// Called every frame
void AHJS_BattlePlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bAttack)
	{
		Alpha += (DeltaTime * 2);
		FVector NewLocation = FMath::Lerp(OriginVector, AttackVector, Alpha);
		GetMesh()->SetRelativeLocation(NewLocation);
		if (Alpha > MoveRate)
		{
			Alpha = 0.f;
			bAttack = false;
		}
	} 

	if (bReturn)
	{
		Alpha += (DeltaTime * 2);
		FVector NewLocation = FMath::Lerp(AttackVector, OriginVector, Alpha);
		GetMesh()->SetRelativeLocation(NewLocation);
		if (Alpha > MoveRate)
		{
			Alpha = 0.f;
			bReturn = false;
		}
	}

	MoveToChargingVFX();

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

void AHJS_BattlePlayer::ClientPlayTutorialUI_Implementation()
{
	// 튜토리얼 UI Text를 출력하는 함수.
	//이 말하기 대결, 나 훈장이 참관하겠네 이 서당의 공식 대결 규칙으로 진행하겠다.
	//빈 칸에 들어갈 알맞은 단어를 보기에서 골라 조합하여 읽어야 한다.각자의 어휘력과 발음을 겨룰 수 있는 최고의 규칙이지.
	//제시된 문장에 맞춰 가장 어울리는 단어를 선택해 말해야 해.
	check(MainUI);
	MainUI->StartTutorialUI(0);

}

void AHJS_BattlePlayer::ServerStartTurnToGM_Implementation()
{
	GM = Cast<AHJS_BattleGameMode>(GetWorld()->GetAuthGameMode());
	GM->StartTurn();
}

void AHJS_BattlePlayer::StartTurn_Implementation()
{
	check(MainUI);
	MainUI->TurnStartUIInit();
}

void AHJS_BattlePlayer::ServerQuestionSetting_Implementation()
{
	check(GM)
		GM->QuestionSetting();
}


void AHJS_BattlePlayer::ClientQuestionSetting_Implementation(FBattleQuestionData Data)
{

	// UI에 데이터 세팅해주고
	MainUI->RecordStartUIInit(Data);
	// 녹음 시작
	FString PlayerID = FString::Printf(TEXT("%d"), FMath::RandRange(10000000, 99999999));
	bWin = false;
	StartRecording(PlayerID);
}


void AHJS_BattlePlayer::Punch()
{
	UBattlePlayerAnim* Anim = Cast<UBattlePlayerAnim>(GetMesh()->GetAnimInstance());
	if (Anim)
	{
		Anim->JumpMontageToAttack();
	}
}
void AHJS_BattlePlayer::AddMainUI_Implementation()
{
	if (MainUIFactory)
	{
		MainUI = CreateWidget<UMainUI>(GetWorld(), MainUIFactory);
		if (MainUI)
		{
			MainUI->Me = this;
			MainUI->AddToViewport();
			MainUI->GameStartUIInit();
		}
	}
}

void AHJS_BattlePlayer::LoginSignal_Implementation()
{
	GM = Cast<AHJS_BattleGameMode>(GetWorld()->GetAuthGameMode());

	APlayerController* PC = Cast<APlayerController>(GetController());

	if (PC == nullptr)
	{
		return;
	}
	GM->JoinPlayer(PC);

}

void AHJS_BattlePlayer::PlayResultAnim_Implementation()
{

	GM = Cast<AHJS_BattleGameMode>(GetWorld()->GetAuthGameMode());
	GM->BattleAnimPlay();
}


void AHJS_BattlePlayer::ServerWinnerSet_Implementation()
{
	MulticastWinnerSet();
}

void AHJS_BattlePlayer::MulticastWinnerSet_Implementation()
{
	bWin = true;



}


void AHJS_BattlePlayer::FirebaseLogin_Implementation()
{
	GoogleNetComp->AnonymousLogin();
}
// 각각의 클라이언트에서 Wav 파일을 AI 서버에 전송 후, STT 결과를 내려받고, 그 결과를 Server에 전송
void AHJS_BattlePlayer::SendRecordToAIServer(const FString& Result)
{
	STTResult = Result;
	SendSTTResultToGameMode(STTResult);
}


// STT 통신 결과를 서버에서 GM에 보내는 코드
void AHJS_BattlePlayer::SendSTTResultToGameMode_Implementation(const FString& Result)
{
	// 서버로 만들었지만 방어코드, 서버인지 확인
	if (HasAuthority())
	{
		// GM을 가져와서 저장하기
		GM = Cast<AHJS_BattleGameMode>(GetWorld()->GetAuthGameMode());

		APlayerController* PC = Cast<APlayerController>(GetController());
		if (!PC)
		{
			return;
		}
		GM->SettingPlayerAnswer(Result, PC);
	}
}

void AHJS_BattlePlayer::SetWinnerNum_Implementation(int32 Value)
{
	WinnerNum = Value;
}

// Firebase서버에 wav파일 업로드
void AHJS_BattlePlayer::UploadWavToFirebase_Implementation()
{
	TArray<uint8> OutByteArray;
	FString FileName = RecordFileName + TEXT(".wav");
	FString FilePath = RecordFilePath + FileName;
	// 이긴 쪽의 클라이언트에서 업로드가 완료되면 서버의 다운로드 함수를 부르기
	GoogleNetComp->FileUploadToFirebase(FilePath, FileName);
}

void AHJS_BattlePlayer::ServerDownloadSound_Implementation(const FString& ClientName)
{
	if (HasAuthority())
	{
		MulticastDownloadSound(ClientName);
	}
}

void AHJS_BattlePlayer::MulticastDownloadSound_Implementation(const FString& ClientName)
{
	// WinnerNum이 1인데 서버가 아니면
	if (WinnerNum == 1 && !HasAuthority())
	{
		return;
	}

	// WinnerNum이 0인데 서버라면
	if (WinnerNum == 0 && HasAuthority())
	{
		return;
	}

	FString FileName = ClientName + TEXT(".wav");
	UE_LOG(LogTemp, Warning, TEXT("%s"), *FileName);
	FString SavePath = FPaths::Combine(FPaths::ProjectDir(), TEXT("RecordData/Winner/"));
	SavePath = SavePath + FileName;

	GoogleNetComp->FileDownloadFromFirebase(SavePath, FileName);
}

void AHJS_BattlePlayer::ClientPlaySound(const FString& WinnerFIlePath)
{
	ServerPlaySound(WinnerFIlePath);
}


void AHJS_BattlePlayer::ServerPlaySound_Implementation(const FString& WinnerFIlePath)
{
	MulticastPlaySound(WinnerFIlePath);
}

void AHJS_BattlePlayer::MulticastPlaySound_Implementation(const FString& WinnerFIlePath)
{

	// 파일 읽기
	TArray<uint8> SoundData;
	// WinnerNum이 1인데 서버가 아니면
	if (WinnerNum == 1 && !HasAuthority())
	{
		FString MyFileName = RecordFileName + TEXT(".wav");
		FString MyFilePath = RecordFilePath + MyFileName;
		if (!FFileHelper::LoadFileToArray(SoundData, *MyFilePath))
		{
			UE_LOG(LogTemp, Error, TEXT("파일 읽기 실패!: %s"), *MyFilePath);
			return;
		}
	}
	// WinnerNum이 0인데 서버라면
	else if (WinnerNum == 0 && HasAuthority())
	{
		FString MyFileName = RecordFileName + TEXT(".wav");
		FString MyFilePath = RecordFilePath + MyFileName;
		if (!FFileHelper::LoadFileToArray(SoundData, *MyFilePath))
		{
			UE_LOG(LogTemp, Error, TEXT("파일 읽기 실패!: %s"), *MyFilePath);
			return;
		}
	}
	else
	{
		if (!FFileHelper::LoadFileToArray(SoundData, *WinnerFIlePath))
		{
			UE_LOG(LogTemp, Error, TEXT("파일 읽기 실패!: %s"), *WinnerFIlePath);
			return;
		}
	}
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AHJS_BattlePlayer::StaticClass(), OutActors);

	for (AActor* Actor : OutActors)
	{
		AHJS_BattlePlayer* Player = Cast<AHJS_BattlePlayer>(Actor);
		Player->WinnerNum = -1;
	}

	// WAV 헤더에서 필요한 정보 추출 (예: 샘플 속도, 채널 수, 등)
	int32 SampleRate = *(int32*)&SoundData[24]; // WAV 파일의 샘플 속도
	int16 NumChannels = *(int16*)&SoundData[22]; // WAV 파일의 채널 수
	int32 DataSize = *(int32*)&SoundData[40]; // 데이터 크기

	// USoundWaveProcedural로 동적 사운드 웨이브 생성
	USoundWaveProcedural* SoundWave = NewObject<USoundWaveProcedural>();
	SoundWave->SetSampleRate(SampleRate);
	SoundWave->NumChannels = NumChannels;
	SoundWave->Duration = static_cast<float>(DataSize) / (SampleRate * NumChannels * sizeof(int16));

	// 오디오 데이터를 SoundWave에 할당
	TArray<uint8> PCMData(SoundData.GetData() + 44, DataSize); // 44바이트 이후가 오디오 데이터
	SoundWave->QueueAudio(PCMData.GetData(), PCMData.Num());

	UGameplayStatics::PlaySound2D(GetWorld(), SoundWave);

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

	bAttack = true;
	Alpha = 0.f;

	GetWorldTimerManager().SetTimer(PunchHandle, this, &AHJS_BattlePlayer::Punch, 10.f, false);
}

void AHJS_BattlePlayer::StartRecording_Implementation(const FString& PlayerID)
{
	bWin = false;

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
		GetWorldTimerManager().SetTimer(RecordHandle, this, &AHJS_BattlePlayer::StopRecording, 10.f, false);
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
		MyRecord = UAudioMixerBlueprintLibrary::StopRecordingOutput(GetWorld(), EAudioRecordingExportType::WavFile, RecordFileName, RecordFilePath, RecordSound);
		// AI 서버에 보내는 함수
		//SendRecordToAIServer(TEXT("0000"));
		// USoundwave to binery
		GetWorldTimerManager().SetTimer(AINetTimerHandle, this, &AHJS_BattlePlayer::AINetReq, 0.3f, false);
		check(MainUI);
		MainUI->LineText = TEXT("판독중...");
	}
}

void AHJS_BattlePlayer::AINetReq()
{
	FString FileName = RecordFileName + TEXT(".wav");
	FString FilePath = RecordFilePath + FileName;
	AINetComp->FileSendToAIServer(FilePath);
}

void AHJS_BattlePlayer::MoveToChargingVFX()
{
	if (ChargingRightHandComp)
	{
		FVector RightHandLocation = GetMesh()->GetSocketLocation(FName("middle_01_r"));
		// 나이아가라 파라미터에 손 위치 전달
		ChargingRightHandComp->SetVectorParameter(FName("HandOffset"), RightHandLocation);
	}

	if (ChargingLeftHandComp)
	{
		FVector LeftHandLocation = GetMesh()->GetSocketLocation(FName("middle_01_l"));
		// 나이아가라 파라미터에 손 위치 전달
		ChargingLeftHandComp->SetVectorParameter(FName("HandOffset"), LeftHandLocation);
	}
}


void AHJS_BattlePlayer::PlayerHit()
{
	UBattlePlayerAnim* Anim = Cast<UBattlePlayerAnim>(GetMesh()->GetAnimInstance());
	check(Anim);
	Anim->PlayHitMontage();
}

void AHJS_BattlePlayer::ServerPlayerHit_Implementation()
{
}

void AHJS_BattlePlayer::MulticastPlayerHit_Implementation()
{
}

void AHJS_BattlePlayer::OnMyTakeDamage(int32 Damage)
{
	HP--;
	// HP를 깎는 게 내가 관리하고 있는 객체라면 Player1의 체력을 깎기
	if (IsLocallyControlled())
	{
		check(MainUI);
		MainUI->SetHP(1, HP);
	}
	else
	{
		AHJS_BattlePlayer* MainPlayer = Cast<AHJS_BattlePlayer>(GetWorld()->GetFirstPlayerController()->GetCharacter());
		check(MainPlayer);
		MainPlayer->MainUI->SetHP(2, HP);
	}

	// HP를 깎는 게 내가 관리하고 있는 객체가 아니라면 Player2의 체력을 깎고
	if (HP <= 0)
	{
		// 죽음.
		return;
	}
}

void AHJS_BattlePlayer::ShowGameEndUI()
{

}

void AHJS_BattlePlayer::ClientMainTextSet_Implementation(const FString& Text)
{
	check(MainUI);
	MainUI->LineText = Text;
}
