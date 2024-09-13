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

