// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "KJH_Player.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FInputBindingSignature, class UEnhancedInputComponent*);
DECLARE_MULTICAST_DELEGATE(FEndedSitSignature);

UCLASS()
class KINGSEJONG_API AKJH_Player : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AKJH_Player();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public: 
	FInputBindingSignature OnInputBindingDelegate;
	//UPROPERTY(BlueprintAssignable)
	FEndedSitSignature OnEndSitDelegate;

public:
	// Camera
	UPROPERTY(EditDefaultsOnly)
	class UCameraComponent* CameraComp;

	// Player 상태
	UPROPERTY(Replicated)
	bool bIsSit;

private:
	UPROPERTY(EditDefaultsOnly)
	class USpringArmComponent* SpringArmComp;

	// Input
	UPROPERTY(EditDefaultsOnly)
	class UInputMappingContext* IMC_Player;
	UPROPERTY(EditDefaultsOnly)
	class UInputAction* IA_Move;
    UPROPERTY(EditDefaultsOnly)
    class UInputAction* IA_Look;
	UPROPERTY(EditDefaultsOnly)
	class UInputAction* IA_Jump;

	// Animation
	UPROPERTY(EditDefaultsOnly)
	class UKJH_PlayerAnimInstance* PlayerAnim;

	// Component
	UPROPERTY(EditDefaultsOnly)
	class UKJH_PlayerInteraction* InteractionComp;
	UPROPERTY(EditDefaultsOnly)
	class UKJH_VoiceRecorder* VoiceRecorderComp;
	


private:
	void OnActionMove(const FInputActionValue& value);
	void OnActionLook(const FInputActionValue& value);
	void OnActionJump(const FInputActionValue& value);

	void SetIsSit(bool bValue);

public:
	UFUNCTION(BlueprintCallable)
	void OnStartSit(FVector TargetLoc, FRotator TargetRot);
	UFUNCTION(BlueprintCallable)
	void OnEndSit();

	void SetPlayerPosition(FTransform TargetTransform);
};
