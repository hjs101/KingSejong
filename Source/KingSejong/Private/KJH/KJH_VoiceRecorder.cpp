// Fill out your copyright notice in the Description page of Project Settings.


#include "KJH/KJH_VoiceRecorder.h"
#include "AudioCaptureComponent.h"
#include "AudioMixerBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "KJH/API/KJH_HttpManager.h"
#include "KJH/API/KJH_FileDataLib.h"

// Sets default values for this component's properties
UKJH_VoiceRecorder::UKJH_VoiceRecorder()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UKJH_VoiceRecorder::BeginPlay()
{
	Super::BeginPlay();

	AudioCaptureComp = GetOwner()->GetComponentByClass<UAudioCaptureComponent>();

	// HttpManager
	HttpManager = Cast<AKJH_HttpManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AKJH_HttpManager::StaticClass()));


	
}

//FString UKJH_VoiceRecorder::GetLocalRecodingFilePath()
//{
//	FString FilePath = FPaths::Combine(FPaths::ProjectSavedDir(), SaveFileDir, SaveFileName);
//	
//	return FilePath + FString(TEXT(".wav"));
//}

bool UKJH_VoiceRecorder::OnStartRecord()
{
	if(AudioCaptureComp == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor에 AudioCaptureComp가 없습니다."));
		return false;
	}

	if ( AudioCaptureComp->IsPlaying() )
	{
		UE_LOG(LogTemp, Warning, TEXT("이미 음성 녹음 중입니다."));
		return false;
	}

	UE_LOG(LogTemp, Warning, TEXT("녹음 시작 시도!"));


	AudioCaptureComp->Start();
	UAudioMixerBlueprintLibrary::StartRecordingOutput(GetWorld(), 0, SoundSubmix);
	
	UE_LOG(LogTemp, Warning, TEXT("OnStartRecord!!"));

	return true;
}

bool UKJH_VoiceRecorder::OnStopRecord()
{
	if ( AudioCaptureComp == nullptr )
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor에 AudioCaptureComp가 없습니다."));
		return false;
	}

	if ( AudioCaptureComp->IsPlaying() == false )
	{
		UE_LOG(LogTemp, Warning, TEXT("녹음 중이 아닙니다."));
		return false;
	}

	AudioCaptureComp->Stop();

	FString FileDir = UKJH_FileDataLib::GetSaveWavFileDirectory();

	UAudioMixerBlueprintLibrary::StopRecordingOutput(GetWorld(), EAudioRecordingExportType::WavFile, SaveFileName, FileDir, SoundSubmix);

	UE_LOG(LogTemp, Warning, TEXT("OnStopRecord!!"));

	return true;
}

void UKJH_VoiceRecorder::SendToChatbot()
{
	// 질문 API 호출
	if ( HttpManager )
	{
		// 텍스트 소통
		//FText text = EditTextBox_Question->GetText();
		//HttpManager->Req_BookAnswer(TEXT(""), text.ToString());

		FString filePath = UKJH_FileDataLib::GetSaveWavFilePath(SaveFileName);

		// 파일 전송
		HttpManager->Req_AskToChatbot(TEXT(""), filePath);
	}

}