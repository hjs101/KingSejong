// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "KJH_HttpHandler.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnResponseBookAnswerSignature, FString);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class KINGSEJONG_API UKJH_HttpHandler : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UKJH_HttpHandler();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	FOnResponseBookAnswerSignature OnResponseBookAnswerDelegate;

private:
	// 서버 URL
	const FString ServerURL = "http://meta-ai.iptime.org:61457/janghoon";


public:

	void Req_BookAnswer(FString BookName, FString Question);

private:

	void OnRes_BookAnswer(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);


		
};
