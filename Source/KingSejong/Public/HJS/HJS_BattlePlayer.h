// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BattleQuestionStruct.h"
#include "HJS_BattlePlayer.generated.h"

UCLASS()
class KINGSEJONG_API AHJS_BattlePlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AHJS_BattlePlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	
    // 녹음 시작 함수
    UFUNCTION(Client, Reliable)
    void StartRecording(const FString& PlayerID);

    // 녹음 종료 함수
    UFUNCTION(Client, Reliable)
    void StopRecording();


	UFUNCTION()
	void Attack();

	UFUNCTION(Server, Reliable)
	void ServerAttack();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastAttack();


	// 게임 시스템 구조 만들기
	// 녹음 시간은 15초.

    void SendRecordToAIServer(const FString& Result);
	FString STTResult = TEXT("안녕하세요");
	// 2. STT 결과 텍스트를 서버(게임모드)에서 체크하여 승자를 가려내고, 승자의 어택 함수를 멀티캐스트로 발동시킨다.
	UFUNCTION(Server, Reliable)
	void SendSTTResultToGameMode(const FString& Result);
	// 3. Wav 파일은 각각의 클라이언트에 있지만, 서버를 통해 두 플레이어 모두에게 들려야 하므로
	// 이긴 쪽의 사운드 데이터를 서버 및 다른 클라이언트에게 넘겨줄 필요가 있다.

	// 승패 여부 변수
	UPROPERTY()
	bool bWin = false;

	UFUNCTION(Server , Reliable)
	void ServerWinnerSet();

	UFUNCTION(NetMulticast , Reliable)
	void MulticastWinnerSet();

	// 이긴 쪽의 클라이언트에서 녹음을 업로드하기 (client로 실행)
	UFUNCTION(Client, Reliable)
	void UploadWavToFirebase();
	
	// 이긴 쪽의 클라이언트에서 업로드가 완료되면 서버의 다운로드 함수를 부르기
	UFUNCTION(Server, Reliable)
	void ServerDownloadSound(const FString& ClientName);
	// 서버는 다운로드 함수를 멀티캐스트로 작동하기.
	UFUNCTION(NetMulticast, Reliable)
	void MulticastDownloadSound(const FString& ClientName);

	// 다운로드 함수는 두 개의 클라이언트 중 한 개만 작동 ( 이긴쪽은 다운로드 할 필요가 없으므로 )
	UFUNCTION()
	void ClientPlaySound(const FString& WinnerFIlePath);

	// 진 쪽의 다운로드 함수(실제로 다운로드를 하는 함수)가 끝나는 시점에 서버의 플레이 함수를 부르기
	UFUNCTION(Server, Reliable)
	void ServerPlaySound(const FString& WinnerFIlePath);
	
	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlaySound(const FString& WinnerFIlePath);
	// 서버의 플레이 함수는 멀티캐스트 함수로 음성 파일을 재생하기.

	// 4. 애니메이션을 재생하면서 Wav에 녹음된 사운드 데이터를 재생한다.
	
	// 5. 패자의 경우 승자의 어택의 Notify 시점에 피격 애니메이션을 재생하도록 한다.
	UFUNCTION()
	void PlayerHit();

	UFUNCTION(Server, Reliable)
	void ServerPlayerHit();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayerHit();
	
	UFUNCTION(NetMulticast,Reliable)
	void SetWinnerNum(int32 Value);
	// 특이사항
	// 지금은 지금은 웹 API 통신이 안되는 상태이기 때문에 임시 텍스트를 이용해 승자와 패자를 확실히 가리기
	// 음성파일을 주고받는 과정에서 딜레이가 있을 수 있기 때문에, 두 플레이어 모두 준비가 되었을 때 시작하도록 하는 조치가 필요하다..
	int32 WinnerNum = -1;

	FString RecordFilePath = "";

	FString RecordFileName = "";

	FString WinnerSavePath = "";

	bool bReturn = false;
	
	UFUNCTION(Server, Reliable)
	void PlayResultAnim();

	UFUNCTION(Client, Reliable)
	void AddMainUI();

	UFUNCTION(client, Reliable)
	void ClientMainTextSet(const FString& Text);

	// 게임 시작 과정

	// 1. 훈장님이 나와 튜토리얼을 진행함 ( 클라이언트로 )
	// 2. 모든 플레이어가 튜토리얼을 끝까지 마쳤으면 서버에 준비됐음을 알림 (서버로)


	// 플레이어의 UI 텍스트 재생 함수를 출력하기
	UFUNCTION(Client, Reliable)
	void ClientPlayTutorialUI();

	// 서버에 튜토리얼을 모두 읽었음을 알리는 함수
	UFUNCTION(Server, Reliable)
	void ServerStartTurnToGM();
	
	// 4. 클라이언트는 3초의 카운트 다운을 진행하고, 문제를 띄우며 녹음을 시작함.
	UFUNCTION(Client, Reliable)
	void StartTurn();

	UFUNCTION(Server, Reliable)
	void ServerQuestionSetting();
	UFUNCTION(Client, Reliable)
	void ClientQuestionSetting(FBattleQuestionData Data);
	// 5. 녹음 결과에 따라 승자와 패자가 갈림

	void OnMyTakeDamage(int32 Damage);

	UPROPERTY()
	int32 MaxHP = 3;

	UPROPERTY()
	int32 HP = MaxHP;

	void ShowGameEndUI();

private:
	bool bIsRecording = false;

	// 테스트를 위한 VFX
	UPROPERTY(EditAnywhere, Category="Test")
	class UParticleSystem* TestVFX;

	UPROPERTY(EditAnywhere)
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(EditAnywhere)
	class UCameraComponent* CameraComp;

    UPROPERTY(VisibleAnywhere, Category = "Audio")
    class UAudioCaptureComponent* AudioCapture;

	UPROPERTY(EditAnywhere, Category = "Audio")
	class USoundSubmix* RecordSound;

	UPROPERTY(EditDefaultsOnly, Category="Net")
	class UAINet* AINetComp;

	UPROPERTY(EditAnywhere, Category="VFX")
	class UNiagaraComponent* ChargingRightHandComp;
	UPROPERTY(EditAnywhere, Category="VFX")
	class UNiagaraComponent* ChargingLeftHandComp;

	FTimerHandle RecordHandle;

	int32 MyNum;

	FTimerHandle PunchHandle;
	UFUNCTION()
	void Punch();

	UPROPERTY()
	class UVoiceReceiver* VoiceReceiver;

	UPROPERTY()
	class UGoogleNet* GoogleNetComp;

	TArray<uint8> VoiceDataArray;
	
	UFUNCTION(Client, Reliable)
	void FirebaseLogin();

	FString PlayerRecordID;

	UPROPERTY()
	class USoundWave* MyRecord;

	UPROPERTY();
	class AHJS_BattlePlayer* YouActor;

	float MoveRate = 1.f;
	float Alpha = 0.f;
	bool bAttack = false;

	FVector OriginVector = FVector(0.f,0.f,-90.f);
	UPROPERTY(EditAnywhere)
	FVector AttackVector = FVector(25.f,-50.f,-90.f);

	UFUNCTION(Server, Reliable)
	void LoginSignal();

	FTimerHandle JoinTimerHandle;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UMainUI> MainUIFactory;

	UPROPERTY()
	UMainUI* MainUI;

	UPROPERTY()
	class AHJS_BattleGameMode* GM;

	FTimerHandle AINetTimerHandle;
	
	UFUNCTION()
	void AINetReq();

	UFUNCTION()
	void MoveToChargingVFX();
};
