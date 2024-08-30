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
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 클라이언트 입력 처리 함수 선언
	void TeleportForward(float Speed);

	// 서버 함수 선언
	UFUNCTION(Server, Reliable)
	void ServerTeleportForward(float Speed);
	void ServerTeleportForward_Implementation(float Speed);

	//멀티캐스트함수
	UFUNCTION(NetMulticast, Reliable)
	void MulticastTeleportForward(float Speed);
	void MulticastTeleportForward_Implementation(float Speed);


};
