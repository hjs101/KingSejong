// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InputActionValue.h"
#include "KJH_PlayerInteraction.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class KINGSEJONG_API UKJH_PlayerInteraction : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UKJH_PlayerInteraction();
	 
protected:
	virtual void InitializeComponent() override;

	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY()
	class AKJH_Player* MyActor;

	UPROPERTY()
	class UCameraComponent* CameraComp;

	UPROPERTY()
	class AKJH_InteractiveActor* HitActor;

	UPROPERTY(EditDefaultsOnly)
	class UInputAction* IA_Interaction;

	UPROPERTY(EditDefaultsOnly)
	float TraceDistance = 5000;

	// UI
    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<class UUserWidget> KeyGuideFactory;

    UPROPERTY()
    class UUserWidget* KeyGuideWidget;

	bool bHitActor = false;

		
private:

	void SetupInputBinding(class UEnhancedInputComponent* Input);
	void OnActionInteraction(const FInputActionValue& value);

	void CreateKeyGuide();
	void SetActiveKeyGuide(bool bValue);

	bool IsInteractableActor(class AKJH_InteractiveActor* OtherActor);

public:
	UFUNCTION(Server, Reliable)
	void ServerRPCInteractiveActor(class AKJH_InteractiveActor* TargetActor , class AKJH_Player* PlayerActor);


};
