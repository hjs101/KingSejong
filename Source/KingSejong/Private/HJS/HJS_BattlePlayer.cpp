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
#include "Components/AudioComponent.h"
#include "HAL/PlatformFileManager.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "../KingSejong.h"
#include "JJH/JJH_GameInstance.h"

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
	ChargingRightHandComp->SetupAttachment(GetMesh());
	ChargingRightHandComp->SetAutoActivate(false);  // 기본적으로 비활성화

	ChargingLeftHandComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ChargingLeftHandComp"));
	ChargingLeftHandComp->SetupAttachment(GetMesh());
	ChargingLeftHandComp->SetAutoActivate(false);  // 기본적으로 활성화

	RecordUIComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("RecordUIComp"));
	RecordUIComp->SetupAttachment(RootComponent);

	bReplicates = true;
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
		GetWorldTimerManager().SetTimer(JoinTimerHandle, this, &AHJS_BattlePlayer::LoginSignal, 1.f, true);
	}
	AudioCapture->OnAudioEnvelopeValue.AddDynamic(this,&AHJS_BattlePlayer::OnChangeEnvValue);
	RecordUIComp->SetVisibility(false);

	UJJH_GameInstance* GameIns = Cast<UJJH_GameInstance>(GetWorld()->GetGameInstance());

	if (IsLocallyControlled())
	{
		ServerSetMesh(GameIns->CharacterMeshIndex);
	}
	else
	{
		GetMesh()->SetSkeletalMesh(GameIns->CharacterList[MyMeshIndex]);
	}
}

void AHJS_BattlePlayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AHJS_BattlePlayer, MyMeshIndex);
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

	if (RecordUIComp && RecordUIComp->GetVisibleFlag())
	{
		// 0번 플레이어 ( 클라이언트든 서버든 자기 자신 )
		FVector CameraLoc = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetCameraLocation();
		FVector Dir = CameraLoc - RecordUIComp->GetComponentLocation();
		Dir.Z = 0.f;

		RecordUIComp->SetWorldRotation(Dir.GetSafeNormal().ToOrientationRotator());
	}
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

void AHJS_BattlePlayer::OnChangeEnvValue(float Value)
{
	if (!IsLocallyControlled())
	{
		return;
	}

	Value = UKismetMathLibrary::Log(Value, 10.0);

	Value = UKismetMathLibrary::MapRangeClamped(Value,-2.7,-0.7,0.0,1.0);

	if (Value >= 0.2f)
	{
		ServerSetShowRecordComp(true);
	}
	else
	{
		ServerSetShowRecordComp(false);
	}
}

void AHJS_BattlePlayer::ServerSetShowRecordComp_Implementation(bool Value)
{
	RecordUIComp->SetVisibility(Value);
}

void AHJS_BattlePlayer::ServerSetMesh_Implementation(int32 MeshIndex)
{
	if (MeshIndex == -1)
	{
		MyMeshIndex = 0;
	}
	else
	{
		MyMeshIndex = MeshIndex;
	}
	MulticastSetMesh(MyMeshIndex);
}

void AHJS_BattlePlayer::MulticastSetMesh_Implementation(int32 MeshIndex)
{
	UJJH_GameInstance* GameIns = Cast<UJJH_GameInstance>(GetWorld()->GetGameInstance());
	PRINTLOG(TEXT("%d"), MeshIndex);
	check(GameIns);
	GetMesh()->SetSkeletalMesh(GameIns->CharacterList[MeshIndex]);
}

void AHJS_BattlePlayer::ClientExitRoom_Implementation()
{
	UJJH_GameInstance* GameIns = Cast<UJJH_GameInstance>(GetWorld()->GetGameInstance());
	check(GameIns);
	GameIns->ExitSession();
}

void AHJS_BattlePlayer::Punch()
{
	UBattlePlayerAnim* Anim = Cast<UBattlePlayerAnim>(GetMesh()->GetAnimInstance());
	if (Anim)
	{
		Anim->JumpMontageToAttack();
	}
	ChargingRightHandComp->Deactivate();
	ChargingLeftHandComp->Deactivate();
}

void AHJS_BattlePlayer::AddMainUI_Implementation()
{
	if (MainUIFactory)
	{
		MainUI = CreateWidget<UMainUI>(GetWorld(), MainUIFactory);
		if (MainUI)
		{
			MainUI->SettingPlayer(this);
			MainUI->AddToViewport();
			MainUI->GameStartUIInit();
			MainUI->SetInitHP(MaxHP);
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

	GetWorldTimerManager().ClearTimer(JoinTimerHandle);

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
		// 플랫폼 파일 시스템 접근
		IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
		FString MyFileName = RecordFileName + TEXT(".wav");
		FString MyFilePath = RecordFilePath + MyFileName;
		FString DestinationFilePath = FPaths::Combine(FPaths::ProjectDir(), TEXT("RecordData/Winner/")) + MyFileName;
		// 원본 파일 경로가 존재하는지 확인
		if (PlatformFile.FileExists(*MyFilePath))
		{
			// 파일 복사 시도
			bool bSuccess = PlatformFile.CopyFile(*DestinationFilePath, *MyFilePath);

			// 결과 출력
			if (bSuccess)
			{
				UE_LOG(LogTemp, Log, TEXT("파일이 성공적으로 복사되었습니다: %s -> %s"), *MyFilePath, *DestinationFilePath);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("파일 복사 실패: %s -> %s"), *MyFilePath, *DestinationFilePath);
			}
		}
		return;
	}

	// WinnerNum이 0인데 서버라면
	if (WinnerNum == 0 && HasAuthority())
	{
		// 플랫폼 파일 시스템 접근
		IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
		FString MyFileName = RecordFileName + TEXT(".wav");
		FString MyFilePath = RecordFilePath + MyFileName;
		FString DestinationFilePath = FPaths::Combine(FPaths::ProjectDir(), TEXT("RecordData/Winner/")) + MyFileName;
		// 원본 파일 경로가 존재하는지 확인
		if (PlatformFile.FileExists(*MyFilePath))
		{
			// 파일 복사 시도
			bool bSuccess = PlatformFile.CopyFile(*DestinationFilePath, *MyFilePath);

			// 결과 출력
			if (bSuccess)
			{
				UE_LOG(LogTemp, Log, TEXT("파일이 성공적으로 복사되었습니다: %s -> %s"), *MyFilePath, *DestinationFilePath);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("파일 복사 실패: %s -> %s"), *MyFilePath, *DestinationFilePath);
			}
		}
		return;
	}

	FString FileName = ClientName + TEXT(".wav");
	UE_LOG(LogTemp, Warning, TEXT("%s"), *FileName);
	FString SavePath = FPaths::Combine(FPaths::ProjectDir(), TEXT("RecordData/Winner/"));
	SavePath = SavePath + FileName;

	GoogleNetComp->FileDownloadFromFirebase(SavePath, FileName);
}

void AHJS_BattlePlayer::ClientPlaySound(const FString& FileName)
{
	ServerPlaySound(FileName);
}

void AHJS_BattlePlayer::ServerPlaySound_Implementation(const FString& FileName)
{
	MulticastPlaySound(FileName);
}

void AHJS_BattlePlayer::MulticastPlaySound_Implementation(const FString& FileName)
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
		FString WinnerFilePath = FPaths::Combine(FPaths::ProjectDir(), TEXT("RecordData/Winner/")) + FileName;
		if (!FFileHelper::LoadFileToArray(SoundData, *WinnerFilePath))
		{
			UE_LOG(LogTemp, Error, TEXT("파일 읽기 실패!: %s"), *WinnerFilePath);
			return;
		}
	}
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AHJS_BattlePlayer::StaticClass(), OutActors);

	for (AActor* Actor : OutActors)
	{
		AHJS_BattlePlayer* Player = Cast<AHJS_BattlePlayer>(Actor);
		Player->WinnerNum = -1;
		Player->GetMesh()->SetRenderCustomDepth(false);
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
	UGameplayStatics::PlaySound2D(this, SoundWave, 200.f);

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

	if (!IsLocallyControlled())
	{
		GetMesh()->SetRenderCustomDepth(true);
	}

	ChargingRightHandComp->Activate();
	ChargingLeftHandComp->Activate();

	bAttack = true;
	Alpha = 0.f;

	GetWorldTimerManager().SetTimer(PunchHandle, this, &AHJS_BattlePlayer::Punch, 7.f, false);
}

void AHJS_BattlePlayer::StartRecording_Implementation(const FString& PlayerID)
{
	bWin = false;
	MainUI->SetTalkCanvasVisiblity(true);
	FirebaseLogin();
	if (PlayerRecordID == "")
	{
		PlayerRecordID = PlayerID;
	}

	
	if (!bIsRecording)
	{
		bIsRecording = true;
		AudioCapture->Start();  // 오디오 캡처 시작
		UAudioMixerBlueprintLibrary::StartRecordingOutput(GetWorld(), 0.f, RecordSound);
		GetWorldTimerManager().SetTimer(RecordHandle, this, &AHJS_BattlePlayer::StopRecording, 7.f, false);
	}
}

void AHJS_BattlePlayer::StopRecording_Implementation()
{

	if (bIsRecording)
	{
		bIsRecording = false;
		AudioCapture->Stop();  // 오디오 캡처 중지
		RecordFileName = FString::Printf(TEXT("%s_Record"), *PlayerRecordID);
		MyRecord = UAudioMixerBlueprintLibrary::StopRecordingOutput(GetWorld(), EAudioRecordingExportType::WavFile, RecordFileName, RecordFilePath, RecordSound);
		// AI 서버에 보내는 함수
		// USoundwave to binery
		GetWorldTimerManager().SetTimer(AINetTimerHandle, this, &AHJS_BattlePlayer::AINetReq, 1.f, true);
		check(MainUI);
		MainUI->LineText = TEXT("판독중이니 잠시만 기다려주시게.");
		MainUI->SetTalkCanvasVisiblity(false);
		RecordUIComp->SetVisibility(false);
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
		// HP를 깎는 게 내가 관리하고 있는 객체가 아니라면 Player2의 체력을 깎고
		AHJS_BattlePlayer* MainPlayer = Cast<AHJS_BattlePlayer>(GetWorld()->GetFirstPlayerController()->GetCharacter());
		check(MainPlayer);
		MainPlayer->MainUI->SetHP(2, HP);
	}

	if (HP <= 0)
	{
		// 죽음.
		GetWorldTimerManager().SetTimer(EndGameTimerHandle,this, &AHJS_BattlePlayer::OnDIe,3.f,false);
		
		return;
	}
}

void AHJS_BattlePlayer::OnDIe()
{
	// 이건 내가 죽은 것이기 때문에 패배 UI 띄우기
	if (IsLocallyControlled())
	{
		ShowGameEndUI(false);
		UGameplayStatics::PlaySound2D(GetWorld(), LoseSound, 0.5f);
	}
	else
	{
		// 아니면 상대가 죽은 것이기 때문에 승리 UI 띄우기
		AHJS_BattlePlayer* MainPlayer = Cast<AHJS_BattlePlayer>(GetWorld()->GetFirstPlayerController()->GetCharacter());
		check(MainPlayer);
		MainPlayer->ShowGameEndUI(true);
		UGameplayStatics::PlaySound2D(GetWorld(),VictorySound, 0.5f);
	}
}

void AHJS_BattlePlayer::ShowGameEndUI(bool bVictory)
{
	if (MainUI)
	{
		MainUI->ShowEndGameUI(bVictory);
		APlayerController* PC = Cast<APlayerController>(GetController());
		PC->SetShowMouseCursor(true);
	}
}

void AHJS_BattlePlayer::PlayBGM_Implementation(int32 BGMNum)
{
	switch (BGMNum)
	{
	case 0:
		UGameplayStatics::PlaySound2D(GetWorld(), BGM0);
		break;
	case 1:
		UGameplayStatics::PlaySound2D(GetWorld(), BGM1);
		break;
	case 2:
		UGameplayStatics::PlaySound2D(GetWorld(), BGM2);
		break;
	}
}

void AHJS_BattlePlayer::ServerRestartGame_Implementation()
{
	// 게임 모드에 재대결 요청
	check(GM);
	GM->ReqRestartGame();
}

void AHJS_BattlePlayer::ServerExitGame_Implementation()
{
	// 게임 모드에 종료 요청
	GM->ReqExitGame();
}

void AHJS_BattlePlayer::ClientEndUISetting_Implementation(const FString& NewText)
{
	check(MainUI);
	MainUI->SettingEndGameUIText(NewText);
}

void AHJS_BattlePlayer::ClientMainTextSet_Implementation(const FString& Text)
{
	check(MainUI);
	MainUI->LineText = Text;

	// 2초 뒤에 MainUI 사라지게 하기
	FTimerHandle Handle;
	
	GetWorldTimerManager().SetTimer(Handle, [this]() {
		MainUI->HideLineBox();
	},2.f,false);

}
