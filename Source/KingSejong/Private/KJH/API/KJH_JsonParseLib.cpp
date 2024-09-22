// Fill out your copyright notice in the Description page of Project Settings.


#include "KJH/API/KJH_JsonParseLib.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonWriter.h"


const FString UKJH_JsonParseLib::TEXT_KEY = FString(TEXT("text_response"));
const FString UKJH_JsonParseLib::AUDIO_ID_KEY = FString(TEXT("audio_file_id"));
const FString UKJH_JsonParseLib::AUDIO_DATA_KEY = FString(TEXT("hoonjang_audio"));

FString UKJH_JsonParseLib::MakeJson(const TMap<FString, FString> source)
{
	// source를 JsonObject 형식으로 만든다.
	TSharedPtr<FJsonObject> jsonObject = MakeShareable(new FJsonObject());

	for ( TPair<FString, FString> pair : source )
	{
		jsonObject->SetStringField(pair.Key, pair.Value);
	}

	// writer를 만들어서 JsonObject를 인코딩
	FString json;
	TSharedRef<TJsonWriter<TCHAR>> writer = TJsonWriterFactory<TCHAR>::Create(&json);
	FJsonSerializer::Serialize(jsonObject.ToSharedRef(), writer);

	return json;
}

FString UKJH_JsonParseLib::JsonParseBookAnwser(const FString& json)
{
	TSharedRef<TJsonReader<TCHAR>> reader = TJsonReaderFactory<TCHAR>::Create(json);
	TSharedPtr<FJsonObject> response = MakeShareable(new FJsonObject());

	FString result;
	if ( FJsonSerializer::Deserialize(reader, response) )
	{
		result = response->GetStringField(TEXT("result"));
	}
	return result;
}

TMap<FString, FString> UKJH_JsonParseLib::JsonParseChatbotAnswer(const FString& json)
{
	TSharedRef<TJsonReader<TCHAR>> reader = TJsonReaderFactory<TCHAR>::Create(json);
	TSharedPtr<FJsonObject> response = MakeShareable(new FJsonObject());

	TMap<FString, FString> result;
	if ( FJsonSerializer::Deserialize(reader, response) )
	{
		FString audioId = response->GetStringField(AUDIO_ID_KEY);
		FString text = response->GetStringField(TEXT_KEY);

		result.Add(AUDIO_ID_KEY, audioId);
		result.Add(TEXT_KEY, text);
	}
	return result;
}

FString UKJH_JsonParseLib::JsonParseChatbotAudioData(const FString& json)
{
	TSharedRef<TJsonReader<TCHAR>> reader = TJsonReaderFactory<TCHAR>::Create(json);
	TSharedPtr<FJsonObject> response = MakeShareable(new FJsonObject());

	FString result = TEXT("");
	if ( FJsonSerializer::Deserialize(reader, response) )
	{
		result = response->GetStringField(AUDIO_DATA_KEY);
	}

	return result;
}
