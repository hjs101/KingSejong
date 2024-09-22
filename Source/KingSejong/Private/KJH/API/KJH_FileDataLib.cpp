// Fill out your copyright notice in the Description page of Project Settings.


#include "KJH/API/KJH_FileDataLib.h"

#include "string"

bool UKJH_FileDataLib::SaveBase64ToWavFile(const FString& Base64Data, const FString& FileName)
{
	FString FilePath = GetSaveWavFilePath(FileName);

	// Base64 디코딩
	TArray<uint8> DecodedData;
	if ( !FBase64::Decode(Base64Data, DecodedData) )
	{
		UE_LOG(LogTemp, Error, TEXT("Base64 디코딩에 실패했습니다."));
		return false;
	}

	// WAV 파일로 저장
	if ( FFileHelper::SaveArrayToFile(DecodedData, *FilePath) )
	{
		UE_LOG(LogTemp, Log, TEXT("WAV 파일이 저장되었습니다: %s"), *FilePath);
		return true;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("WAV 파일 저장에 실패했습니다."));
		return false;
	}

    return false;
}

FString UKJH_FileDataLib::GetSaveWavFileDirectory()
{
	return FPaths::Combine(FPaths::ProjectSavedDir(), TEXT("KJH_WavFile"));
}

FString UKJH_FileDataLib::GetSaveWavFilePath(const FString& FileName)
{
	return FPaths::Combine(GetSaveWavFileDirectory(), FileName) + TEXT(".wav");
}


USoundWaveProcedural* UKJH_FileDataLib::CreateSoundWaveToAudioData(const FString& AudioData)
{
	// Base64 디코딩
	TArray<uint8> DecodedData;
	if ( !FBase64::Decode(AudioData, DecodedData) )
	{
		UE_LOG(LogTemp, Error, TEXT("Base64 디코딩에 실패했습니다."));
		return nullptr;
	}
	//// 새로운 SoundWave 객체 생성
	//USoundWaveProcedural* SoundWaveProcedural = NewObject<USoundWaveProcedural>(USoundWaveProcedural::StaticClass());

	//if ( !SoundWaveProcedural )
	//{
	//	UE_LOG(LogTemp, Error, TEXT("SoundWave 객체 생성에 실패했습니다."));
	//	return nullptr;
	//}
	//// 샘플레이트와 채널 설정
	//SoundWaveProcedural->SetSampleRate(44100);  // 예시 샘플레이트
	//SoundWaveProcedural->NumChannels = 2;       // 스테레오

	//// 오디오 데이터를 SoundWaveProcedural로 전달
	//SoundWaveProcedural->QueueAudio(DecodedData.GetData(), DecodedData.Num());


	// WAV 헤더에서 필요한 정보 추출 (예: 샘플 속도, 채널 수, 등)
	int32 SampleRate = *(int32*)&DecodedData[24]; // WAV 파일의 샘플 속도
	int16 NumChannels = *(int16*)&DecodedData[22]; // WAV 파일의 채널 수
	int32 DataSize = *(int32*)&DecodedData[40]; // 데이터 크기

	// USoundWaveProcedural로 동적 사운드 웨이브 생성
	USoundWaveProcedural* SoundWave = NewObject<USoundWaveProcedural>();
	SoundWave->SetSampleRate(SampleRate);
	SoundWave->NumChannels = NumChannels;
	SoundWave->Duration = static_cast<float>(DataSize) / (SampleRate * NumChannels * sizeof(int16));

	// 오디오 데이터를 SoundWave에 할당
	TArray<uint8> PCMData(DecodedData.GetData() + 44, DataSize); // 44바이트 이후가 오디오 데이터
	SoundWave->QueueAudio(PCMData.GetData(), PCMData.Num());


	//return SoundWaveProcedural;
	return SoundWave;
}


FString UKJH_FileDataLib::StringBase64Encode(const FString& str)
{
	// Set 할 때 : FString -> UTF8 (std::string) -> TArray<uint8> -> base64 로 Encode
	std::string utf8String = TCHAR_TO_UTF8(*str);
	TArray<uint8> arrayData = TArray<uint8>(( uint8* ) (utf8String.c_str()),
	utf8String.length());
	return FBase64::Encode(arrayData);
}

FString UKJH_FileDataLib::StringBase64Decode(const FString& str)
{
	// Get 할 때 : base64 로 Decode -> TArray<uint8> -> TCHAR
	TArray<uint8> arrayData;
	FBase64::Decode(str, arrayData);
	std::string ut8String(( char* ) (arrayData.GetData()), arrayData.Num());
	return UTF8_TO_TCHAR(ut8String.c_str());
}
