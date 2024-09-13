// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "KJH_FileDataLib.generated.h"

/**
 * 
 */
UCLASS()
class KINGSEJONG_API UKJH_FileDataLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

public:
	static bool SaveBase64ToWavFile(const FString& Base64Data, const FString& FilePath);

	static FString StringBase64Encode(const FString& Str);
	static FString StringBase64Decode(const FString& Str);

	static FString GetSaveWavFileDirectory();
	static FString GetSaveWavFilePath(const FString& FileName);
};
