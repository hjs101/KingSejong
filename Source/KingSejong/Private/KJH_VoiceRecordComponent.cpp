// Fill out your copyright notice in the Description page of Project Settings.


#include "KJH_VoiceRecordComponent.h"
#include "Sound/SoundSubmix.h"
#include "AudioMixerBlueprintLibrary.h"
#include "AudioMixerDevice.h"


bool UKJH_VoiceRecordComponent::Init(int32& SampleRate)
{
	NumChannels = 1;

#if SYNTHCOMPONENT_EX_OSCILATOR_ENABLED
	// Initialize the DSP objects
	Osc.Init(SampleRate);
	Osc.SetFrequency(440.0f);
	Osc.Start();
#endif // SYNTHCOMPONENT_EX_OSCILATOR_ENABLED

	if(VoiceRecoredSoundSubMix)
		this->SoundSubmix = VoiceRecoredSoundSubMix;

	return true;
}

int32 UKJH_VoiceRecordComponent::OnGenerateAudio(float* OutAudio, int32 NumSamples)
{
#if SYNTHCOMPONENT_EX_OSCILATOR_ENABLED
	// Perform DSP operations here
	for (int32 Sample = 0; Sample < NumSamples; ++Sample)
	{
		OutAudio[Sample] = Osc.Generate();
	}
#endif // SYNTHCOMPONENT_EX_OSCILATOR_ENABLED

	//return NumSamples;
	return Super::OnGenerateAudio(OutAudio, NumSamples);
}

void UKJH_VoiceRecordComponent::SetFrequency(const float InFrequencyHz)
{
#if SYNTHCOMPONENT_EX_OSCILATOR_ENABLED
	// Use this protected base class method to push a lambda function which will safely execute in the audio render thread.
	SynthCommand([this, InFrequencyHz]()
	{
		Osc.SetFrequency(InFrequencyHz);
		Osc.Update();
	});
#endif // SYNTHCOMPONENT_EX_OSCILATOR_ENABLED
}

void UKJH_VoiceRecordComponent::StartRecord()
{
	if (IsRecording()) return;
	this->Start();

	UAudioMixerBlueprintLibrary::StartRecordingOutput(GetWorld(), 0, VoiceRecoredSoundSubMix.Get());

}

void UKJH_VoiceRecordComponent::StopRecord(const bool bOnlyStop)
{
	if (!IsRecording())	return;

	if (!bOnlyStop)
	{
		FString fileDirectoryPath = FPaths::ProjectSavedDir();

		Audio::FMixerDevice* mixerDevice = FAudioDeviceManager::GetAudioMixerDeviceFromWorldContext(GetWorld());

		if (mixerDevice)
		{
			float OutSampleRate;
			float OutChannelCount;

			Audio::FAlignedFloatBuffer& recordedBuffer = mixerDevice->StopRecording(VoiceRecoredSoundSubMix.Get(), OutChannelCount, OutSampleRate);

			if (recordedBuffer.Num() == 0)
			{
				return;
			}

			RecordingData.Reset(new Audio::FAudioRecordingData());
			RecordingData->InputBuffer = Audio::TSampleBuffer<int16>(recordedBuffer, OutChannelCount, OutSampleRate);
			RecordingData->Writer.BeginWriteToWavFile(RecordingData->InputBuffer, TEXT("RecordVoiceFile_Q&A"), fileDirectoryPath, [this]()
			{
					if (VoiceRecoredSoundSubMix && VoiceRecoredSoundSubMix->OnSubmixRecordedFileDone.IsBound())
					{
						VoiceRecoredSoundSubMix->OnSubmixRecordedFileDone.Broadcast(nullptr);
					}
					RecordingData.Reset();
			});
		}
	}
	this->Stop();

	// todo: 델리게이트 연동
}

const bool UKJH_VoiceRecordComponent::IsRecording() const
{
	return IsPlaying();
}
