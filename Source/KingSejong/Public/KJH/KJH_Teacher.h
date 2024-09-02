// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KJH/KJH_InteractiveActor.h"
#include "KJH_Teacher.generated.h"

UENUM()
enum class ETeacherState
{
	Idle,
	Listen,
	Think,
	Speak
};

UCLASS()
class KINGSEJONG_API AKJH_Teacher : public AKJH_InteractiveActor
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	AKJH_Teacher();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void OnBeginInteraction(class AActor* OtherActor) override;
	virtual void OnEndInteraction() override;
	virtual bool IsInteractable() override;;

public:
	UPROPERTY(EditDefaultsOnly)
	class UCapsuleComponent* CapsuleComp;
	UPROPERTY(EditDefaultsOnly)
	class USkeletalMeshComponent* SkeletalMeshComp;

	UPROPERTY(VisibleAnywhere, Replicated)
	ETeacherState TeacherState;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> RecodingWidgetFactory;

	UPROPERTY()
	class UUserWidget* RecodingWidget;

public:
	void SetTeacherState(ETeacherState NewState);

	UFUNCTION(Server, Unreliable)
	void ServerRPCSetTeacherState(ETeacherState NewState);
};
