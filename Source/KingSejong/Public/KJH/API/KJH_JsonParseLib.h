﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "KJH_JsonParseLib.generated.h"

/**
 * 
 */
UCLASS()
class KINGSEJONG_API UKJH_JsonParseLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static const FString TEXT_KEY;
	static const FString AUDIO_ID_KEY;
	static const FString AUDIO_DATA_KEY;


public:
	static FString MakeJson(const TMap<FString, FString> source);

	static FString JsonParseBookAnwser(const FString& json);
	static TMap<FString, FString> JsonParseChatbotAnswer(const FString& json);
	static FString JsonParseChatbotAudioData(const FString& json);

};
