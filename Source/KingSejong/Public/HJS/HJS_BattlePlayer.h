// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
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

    void SendRecordToAIServer();
	FString STTResult = "안녕하세요";
	// 2. STT 결과 텍스트를 서버(게임모드)에서 체크하여 승자를 가려내고, 승자의 어택 함수를 멀티캐스트로 발동시킨다.
	UFUNCTION(Server, Reliable)
	void SendSTTResultToGameMode(const FString& Result);
	// 3. Wav 파일은 각각의 클라이언트에 있지만, 서버를 통해 두 플레이어 모두에게 들려야 하므로
	// 이긴 쪽의 사운드 데이터를 서버 및 다른 클라이언트에게 넘겨줄 필요가 있다.
	// 사운드 파일을 바이트 배열로 변환해서 서버에 전송
	UFUNCTION(Client, Reliable)
	void SendWavToServer();

	UFUNCTION(Server, Reliable)
	void ReceiveServerSoundData(const FString& FileName);
	// 4. 애니메이션을 재생하면서 Wav에 녹음된 사운드 데이터를 재생한다.(재생은 그냥 멀티캐스트로 해도 되나?)
	void ClientPlaySoundData(const TArray<uint8>& SoundData);
	// 5. 패자의 경우 승자의 어택의 Notify 시점에 피격 애니메이션을 재생하도록 한다.
	UFUNCTION()
	void PlayerHit();

	UFUNCTION(Server, Reliable)
	void ServerPlayerHit();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayerHit();
	
	// 특이사항
	// 지금은 지금은 웹 API 통신이 안되는 상태이기 때문에 임시 텍스트를 이용해 승자와 패자를 확실히 가리기
	// 음성파일을 주고받는 과정에서 딜레이가 있을 수 있기 때문에, 두 플레이어 모두 준비가 되었을 때 시작하도록 하는 조치가 필요하다..
private:
	bool bIsRecording = false;

	FString RecordFilePath = "";

	FString RecordFileName = "";

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
};
