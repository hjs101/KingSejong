// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputActionValue.h"
#include "Runner.generated.h"

UCLASS()
class KINGSEJONG_API ARunner : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARunner();

	UPROPERTY(EditAnywhere)
	class USpringArmComponent* CameraBoom;

	UPROPERTY(EditAnywhere)
	class UCameraComponent* FollowCamera;

	UPROPERTY(EditDefaultsOnly)
	class UInputMappingContext* IMC_Runner;

	UPROPERTY(EditAnywhere)
	class UInputAction* JJH_IA_Move;

	void Run(const FInputActionValue& value);

	UPROPERTY(EditAnywhere)
	float RunningSpeed;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:		
	bool bCanDance = false;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//몽타주 플레이 함수
	void PlayRunMontageSection(const FName& SectionName);
	void PlayWinMontage();


	UFUNCTION(Server, Reliable)
	void ServerPlayWinMontage();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayWinMontage();
	void PlayLoseMontage();
	UFUNCTION(Server, Reliable)
	void ServerPlayLoseMontage();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayLoseMontage();

	UPROPERTY(EditAnywhere)
	class UAnimMontage* RunMontage;
	UPROPERTY(EditAnywhere)
	class UAnimMontage* WinMontage;
	UPROPERTY(EditAnywhere)
	class UAnimMontage* LoseMontage;

	// 클라이언트 입력 처리 함수 선언
	void TeleportForward(float Speed);

	// 서버 함수 선언
	UFUNCTION(Server, Reliable)
	void ServerTeleportForward(float Speed, float InputValue);
	void ServerTeleportForward_Implementation(float Speed, float InputValue);

	//멀티캐스트함수
	UFUNCTION(NetMulticast, Reliable)
	void MulticastTeleportForward(float Speed, float InputValue);
	void MulticastTeleportForward_Implementation(float Speed, float InputValue);

	UPROPERTY(ReplicatedUsing = OnRep_RunnerMeshIndex)
	int32 RunnerMeshIndex;

	UFUNCTION()
	void OnRep_RunnerMeshIndex();

	void UpdateMesh();


	UPROPERTY(Replicated)
	FString PlayerUniqueID;

	UPROPERTY(Replicated)
	int32 ControllerMeshIndex;
	
	UFUNCTION()
	void OnRep_MeshIndex();
};
