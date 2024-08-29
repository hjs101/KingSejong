// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AudioCaptureComponent.h"
#include "DSP/Osc.h"
#include "Sound/SampleBufferIO.h"
#include "KJH_VoiceRecordComponent.generated.h"

// ========================================================================
// UKJH_VoiceRecordComponent
// Synth component class which implements USynthComponent
// This is a simple hello-world type example which generates a sine-wave
// tone using a DSP oscillator class and implements a single function to set
// the frequency. To enable example:
// 1. Ensure "SignalProcessing" is added to project's .Build.cs in PrivateDependencyModuleNames
// 2. Enable macro below that includes code utilizing SignalProcessing Oscilator
// ========================================================================

#define SYNTHCOMPONENT_EX_OSCILATOR_ENABLED 0

UCLASS(ClassGroup = Synth, meta = (BlueprintSpawnableComponent))
class KINGSEJONG_API UKJH_VoiceRecordComponent : public UAudioCaptureComponent
{
	GENERATED_BODY()
	
	// Called when synth is created	
	virtual bool Init(int32& SampleRate) override;

	// Called to generate more audio
	virtual int32 OnGenerateAudio(float* OutAudio, int32 NumSamples) override;

	// Sets the oscillator's frequencyca
	UFUNCTION(BlueprintCallable, Category = "Synth|Components|Audio")
	void SetFrequency(const float FrequencyHz = 440.0f);

protected:
#if SYNTHCOMPONENT_EX_OSCILATOR_ENABLED
	// A simple oscillator class. Can also generate Saw/Square/Tri/Noise.
	Audio::FOsc Osc;
#endif // SYNTHCOMPONENT_EX_OSCILATOR_ENABLED

protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class USoundSubmix> VoiceRecoredSoundSubMix;

	TUniquePtr<Audio::FAudioRecordingData> RecordingData;

public:
	UFUNCTION(BlueprintCallable)
	void StartRecord();

	UFUNCTION(BlueprintCallable)
	void StopRecord(const bool bOnlyStop);

	UFUNCTION(BlueprintCallable)
	const bool IsRecording() const;


};