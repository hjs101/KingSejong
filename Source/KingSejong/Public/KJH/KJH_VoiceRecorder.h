// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "KJH_VoiceRecorder.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class KINGSEJONG_API UKJH_VoiceRecorder : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UKJH_VoiceRecorder();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	class UAudioCaptureComponent* AudioCaptureComp;

	UPROPERTY(EditDefaultsOnly)
	class USoundSubmix* SoundSubmix;

	const FString SaveFileName = "RecordVoiceFile_Question";

public:
	UFUNCTION(BlueprintCallable)
	bool OnStartRecord();
	UFUNCTION(BlueprintCallable)
	bool OnStopRecord();

};
