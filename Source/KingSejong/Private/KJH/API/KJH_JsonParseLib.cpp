// Fill out your copyright notice in the Description page of Project Settings.


#include "KJH/API/KJH_JsonParseLib.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonWriter.h"

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
