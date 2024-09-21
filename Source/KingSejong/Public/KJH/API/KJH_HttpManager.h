// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "KJH_HttpManager.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnResponseBookAnswerSignature, FString);
DECLARE_DELEGATE_ThreeParams(FOnResponseAskChatbotAnswerSignature, bool, const FString&, const FString&);
DECLARE_DELEGATE_OneParam(FOnResponseGetAudioDataSignature, const FString&);


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
	FOnResponseGetAudioDataSignature OnResponseGetAudioDataDelegate;
private:

	// 서버 URL (Metaverse)
	//const FString ServerURL = "http://meta-ai.iptime.org:61457/janghoon";
	//const FString WavServerURL = "http://metaai.iptime.org:61457/hoonjang_wav";

	const FString ChatbotTextServerURL = "http://metaai.iptime.org:61457/hoonjang_text";
	const FString ChatbotAudioServerURL = "http://metaai.iptime.org:61457/hoonjang_wav";

	// 임시 서버 URL 
	//const FString TempChatbotTextServerURL = "https://0e16-222-101-50-123.ngrok-free.app/hoonjang_text";
	//const FString TempChatbotAudioServerURL = "https://0e16-222-101-50-123.ngrok-free.app";


	const FString ChatbotFileName = FString(TEXT("Response_Chatbot"));

public:	
	void Req_AskByText(FString BookName, FString Question);
	void Req_AskToChatbot(const FString& BookName, const FString& FilePath);
	void Req_GetChatbotAudioData(const FString& AudioId);

private:
	
	void OnRes_AskByText(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
	void OnRes_AskToChatbot(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
	void OnRes_GetChatbotAudioData(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

};
