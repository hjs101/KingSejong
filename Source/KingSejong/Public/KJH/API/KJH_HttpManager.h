// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "KJH/KJH_QuizManager.h"
#include "KJH_HttpManager.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnResponseBookAnswerSignature, FString);
DECLARE_DELEGATE_ThreeParams(FOnResponseAskChatbotAnswerSignature, bool, const FString&, const FString&);
DECLARE_DELEGATE_OneParam(FOnResponseGetAudioDataSignature, const FString&);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnResponseChatbotQuizDataSignature, TArray<FChatbotQuizData>, TargetData);


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
	UPROPERTY(BlueprintAssignable)
	FOnResponseChatbotQuizDataSignature OnResponseChatbotQuizDataDelegate;

private:

	// 서버 URL (Metaverse)
	const FString ChatbotTextServerURL = TEXT("http://metaai2.iptime.org:61457/hoonjang_text");
	const FString ChatbotAudioServerURL = TEXT("http://metaai2.iptime.org:61457/");
	const FString QuizServerURL = TEXT("http://metaai2.iptime.org:61458/hoonjang_quiz");

	const FString ChatbotFileName = FString(TEXT("Response_Chatbot"));

public:	
	void Req_AskToChatbot(const FString& FilePath);
	void Req_GetChatbotAudioData(const FString& AudioId);

	UFUNCTION(BlueprintCallable)
	void Req_GetChatbotQuizData(const FString&BookName);

private:
	
	void OnRes_AskToChatbot(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
	void OnRes_GetChatbotAudioData(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
	void OnRes_GetChatbotQuizData(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
};
