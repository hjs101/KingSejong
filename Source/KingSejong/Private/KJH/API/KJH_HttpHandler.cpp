// Fill out your copyright notice in the Description page of Project Settings.


#include "KJH/API/KJH_HttpHandler.h"
#include "KJH/API/KJH_JsonParseLib.h"
#include "HttpModule.h"

// Sets default values for this component's properties
UKJH_HttpHandler::UKJH_HttpHandler()
{
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UKJH_HttpHandler::BeginPlay()
{
	Super::BeginPlay();

	FString question = TEXT("인어공주는 마녀에게 뭘 주고 다리를 얻었어?");

	Req_BookAnswer(TEXT(""), question);
}

/// <summary>
/// 책 질문에 대한 답을 요청하는 함수
/// </summary>
/// <param name="BookName"> 책 이름 </param>
/// <param name="Question"> 질문 </param>
void UKJH_HttpHandler::Req_BookAnswer(FString BookName, FString Question)
{
	FHttpModule& httpModule = FHttpModule::Get();
	TSharedRef<IHttpRequest> req = httpModule.CreateRequest();

	TMap<FString, FString> data;
	data.Add(TEXT("query"), Question);

	UE_LOG(LogTemp, Warning, TEXT("Req_BookAnswer Call!!: %s"), *Question);

	// 요청 정보
	req->SetURL(ServerURL);
	req->SetVerb(TEXT("POST"));
	req->SetHeader(TEXT("content-type"), TEXT("application/json"));
	req->SetContentAsString(UKJH_JsonParseLib::MakeJson(data));

	// 응답받을 함수
	req->OnProcessRequestComplete().BindUObject(this, &UKJH_HttpHandler::OnRes_BookAnswer);
	// 서버에 요청
	req->ProcessRequest();
}

void UKJH_HttpHandler::OnRes_BookAnswer(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	UE_LOG(LogTemp, Warning, TEXT("OnRes_BookAnswer Call!!"));

	FString result;
	if ( bConnectedSuccessfully )
	{
		FString res = Response->GetContentAsString();
		result = UKJH_JsonParseLib::JsonParseBookAnwser(res);
		UE_LOG(LogTemp, Warning, TEXT("OnRes_BookAnswer Succeed!! : %s"), *result);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("OnRes_BookAnswer Failed!!"));
	}

	OnResponseBookAnswerDelegate.Broadcast(result);
}

