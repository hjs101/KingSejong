// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "KJH_Player.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FInputBindingDelegate, class UEnhancedInputComponent*)

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

public: 
	FInputBindingDelegate OnInputBindingDelegate;



public:
	// Camera
	UPROPERTY(EditDefaultsOnly)
	class UCameraComponent* CameraComp;

private:
	UPROPERTY(EditDefaultsOnly)
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(EditDefaultsOnly)
	class UKJH_PlayerInteraction* InteractionComp;

	// Input
	UPROPERTY(EditDefaultsOnly)
	class UInputMappingContext* IMC_Player;
	UPROPERTY(EditDefaultsOnly)
	class UInputAction* IA_Move;
    UPROPERTY(EditDefaultsOnly)
    class UInputAction* IA_Look;
	UPROPERTY(EditDefaultsOnly)
	class UInputAction* IA_Jump;

	// πÊ«‚
	FVector Direction;

private:
	void OnActionMove(const FInputActionValue& value);
	void OnActionLook(const FInputActionValue& value);
	void OnActionJump(const FInputActionValue& value);

};
