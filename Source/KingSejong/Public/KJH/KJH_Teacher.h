﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KJH/KJH_InteractiveActor.h"
#include "KJH_Teacher.generated.h"


UENUM()
enum class ETeacherState : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	Listen UMETA(DisplayName = "Listen"),
	Think UMETA(DisplayName = "Think"),
	Answer UMETA(DisplayName = "Answer")
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

	// 현재 상태
	UPROPERTY(VisibleAnywhere, Replicated)
	ETeacherState TeacherState = ETeacherState::Idle;

	UPROPERTY(EditDefaultsOnly)
	class UCapsuleComponent* CapsuleComp;
	UPROPERTY(EditDefaultsOnly)
	class USkeletalMeshComponent* SkeletalMeshComp;

	// UI
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> RecodingWidgetFactory;

	UPROPERTY()
	class UKJH_VoiceRecodingWidget* RecodingWidget;

	UPROPERTY(EditDefaultsOnly, Replicated)
	class UWidgetComponent* StateWidgetComp;

	UPROPERTY(Replicated)
	class UKJH_SpeechBubbleWidget* SpeechBubbleWidget;


public:
	void SetTeacherState(ETeacherState NewState);
	void SetTeacherStateToIdle();
private:
		
	void CreateRecodingWidget();
	void SetVisiblityStateWidget(bool bValue);

	/**/
    UFUNCTION(Client, Unreliable)
    void ClientRPC_CreateRecodingWidget();

	UFUNCTION(Server, Unreliable)
	void ServerRPC_SetTeacherState(ETeacherState NewState);

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastRPC_SetTeacherState(ETeacherState NewState);

	/**/
	void SetSpeechBubbleText(FString Message);

	UFUNCTION(Server, Unreliable)
	void ServerRPC_SetSpeechBubbleText(const FString& Message);

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastRPC_SetSpeechBubbleText(const FString& Message);

	
	void CastSpeechBubbleText();

	void SetSpeechBubbleText_Test();

	
};
