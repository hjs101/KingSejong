// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "KJH_HttpManager.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnResponseBookAnswerSignature, FString);
DECLARE_DELEGATE_ThreeParams(FOnResponseAskChatbotAnswerSignature, bool, const FString&, const FString&);


UCLASS()
class KINGSEJONG_API AKJH_HttpManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AKJH_HttpManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	FOnResponseBookAnswerSignature OnResponseAskByTextDelegate;
	FOnResponseAskChatbotAnswerSignature OnResponseAskChatbotAnswerDelegate;

private:
	// 서버 URL
	const FString ServerURL = "http://meta-ai.iptime.org:61457/janghoon";
	const FString WavServerURL = "http://metaai.iptime.org:61457/hoonjang_wav";

	const FString ChatbotFileName = FString(TEXT("Response_Chatbot"));

public:	
	void Req_AskByText(FString BookName, FString Question);
	void Req_AskToChatbot(const FString& BookName, const FString& FilePath);

private:
	
	void OnRes_AskByText(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
	void OnRes_AskToChatbot(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

};
