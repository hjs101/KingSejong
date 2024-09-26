// Fill out your copyright notice in the Description page of Project Settings.


#include "HJS/AINet.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "HttpModule.h"
#include "HttpFwd.h"
#include "HJS/HJS_BattlePlayer.h"

// Sets default values for this component's properties
UAINet::UAINet()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAINet::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UAINet::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UAINet::FileSendToAIServer(const FString& FilePath)
{
	// 파일 읽기
	TArray<uint8> FileData;
	if ( !FFileHelper::LoadFileToArray(FileData, *FilePath) )
	{
		UE_LOG(LogTemp, Error, TEXT("파일 읽기 실패!: %s"), *FilePath);
		return;
	}
	Me->GetWorldTimerManager().ClearTimer(Me->AINetTimerHandle);
	FString SendText = FBase64::Encode(FileData);
	TMap<FString, FString> Senddata;
	Senddata.Add(TEXT("audio"), SendText);
	
	// HTTP 요청 생성
	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetURL(ServerURL);
	HttpRequest->SetVerb("POST");
	HttpRequest->SetHeader(TEXT("content-type"), TEXT("application/json"));
	HttpRequest->SetContentAsString(MakeJson(Senddata));

	// HTTP 응답 처리
	HttpRequest->OnProcessRequestComplete().BindLambda([ this ] (FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			if ( bWasSuccessful && Response->GetResponseCode() == 200 )
			{
				FString Result;
				UE_LOG(LogTemp, Log, TEXT("Get STTText successfully"));
				FString Res = Response->GetContentAsString();
				Result = JsonParseSTTData(Res);
				UE_LOG(LogTemp, Warning, TEXT("OnRes_BookAnswer Succeed!! : %s"), *Result);
				Me->SendRecordToAIServer(Result);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Get STTText failed!!!"));
				Me->SendRecordToAIServer(TEXT("이런, 통신이 원활하지 않습니다."));
			}
		});

	// 요청 전송
	HttpRequest->ProcessRequest();
}

FString UAINet::MakeJson(const TMap<FString, FString> source)
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

FString UAINet::JsonParseSTTData(const FString& json)
{
	TSharedRef<TJsonReader<TCHAR>> reader = TJsonReaderFactory<TCHAR>::Create(json);
	TSharedPtr<FJsonObject> response = MakeShareable(new FJsonObject());

	FString result;
	if ( FJsonSerializer::Deserialize(reader, response) )
	{
		result = response->GetStringField(TEXT("hoonjang_stt"));
	}
	return result;
}
