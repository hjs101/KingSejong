﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "KJH/API/KJH_HttpManager.h"
#include "KJH/API/KJH_JsonParseLib.h"
#include "HttpModule.h"
#include "KJH/API/KJH_FileDataLib.h"

// Sets default values
AKJH_HttpManager::AKJH_HttpManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AKJH_HttpManager::BeginPlay()
{
	Super::BeginPlay();
	
	//FString question = TEXT("인어공주는 마녀에게 뭘 주고 다리를 얻었어?");

	//Req_BookAnswer(TEXT(""), question);
}

/// <summary>
/// 책 질문에 대한 답을 요청하는 함수
/// </summary>
/// <param name="BookName"> 책 이름 </param>
/// <param name="Question"> 질문 </param>
void AKJH_HttpManager::Req_AskByText(FString BookName, FString Question)
{
	FHttpModule& httpModule = FHttpModule::Get();
	TSharedRef<IHttpRequest> req = httpModule.CreateRequest();
		
	TMap<FString, FString> data;
	data.Add(TEXT("query"), Question);

	// 요청 정보
	req->SetURL(ServerURL);
	req->SetVerb(TEXT("POST"));
	req->SetHeader(TEXT("content-type"), TEXT("application/json"));
	req->SetContentAsString(UKJH_JsonParseLib::MakeJson(data));

	// 응답받을 함수
	req->OnProcessRequestComplete().BindUObject(this, &AKJH_HttpManager::OnRes_AskByText);
	// 서버에 요청
	req->ProcessRequest();

}


void AKJH_HttpManager::OnRes_AskByText(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
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

	OnResponseAskByTextDelegate.Broadcast(result);
}

/// <summary>
/// 파일 경로로 질문하기
/// </summary>
/// <param name="BookName"> 책 이름</param>
/// <param name="FilePath"> 보이스 파일 경로 </param>
void AKJH_HttpManager::Req_AskToChatbot(const FString& BookName, const FString& FilePath)
{
	FHttpModule& httpModule = FHttpModule::Get();
	TSharedRef<IHttpRequest> req = httpModule.CreateRequest();

	// 파일 읽기
	TArray<uint8> FileData;
	if ( !FFileHelper::LoadFileToArray(FileData, *FilePath) )
	{
		UE_LOG(LogTemp, Error, TEXT("파일 읽기 실패!: %s"), *FilePath);
		return;
	}
	// 읽어온 파일을 Base64로 인코딩
	FString EncodingFileData = FBase64::Encode(FileData);

	// 통신할 데이터
	TMap<FString, FString> data;
	data.Add(TEXT("audio"), EncodingFileData);

	UE_LOG(LogTemp, Warning, TEXT("파일 읽기 성공: %s"), *FilePath);

	//UE_LOG(LogTemp, Warning, TEXT("Req_AskChatbotByWavFile Call!!: %s"), *Question);

	// 요청 정보
	req->SetURL(WavServerURL);
	req->SetVerb(TEXT("POST"));
	req->SetHeader(TEXT("content-type"), TEXT("application/json"));
	req->SetContentAsString(UKJH_JsonParseLib::MakeJson(data));

	// 응답 처리
	req->OnProcessRequestComplete().BindUObject(this, &AKJH_HttpManager::OnRes_AskToChatbot);

	// 서버에 요청
	req->ProcessRequest();
}

void AKJH_HttpManager::OnRes_AskToChatbot(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	TMap<FString, FString> result;
	if ( bConnectedSuccessfully )
	{
		FString res = Response->GetContentAsString();
		result = UKJH_JsonParseLib::JsonParseChatbotAnswer(res);

		FString audioData = result[TEXT("hoonjang_audio")];
		FString text = result[TEXT("hoonjang_text")];

		// 챗봇 wav 응답 저장
		bool bIsSaveAudio = UKJH_FileDataLib::SaveBase64ToWavFile(audioData, ChatbotFileName);

		// 통신 성공 호출
		if ( bIsSaveAudio )
		{
			OnResponseAskChatbotAnswerDelegate.ExecuteIfBound(true, audioData, text);
			UE_LOG(LogTemp, Warning, TEXT("OnRes_AskChatbotByFile Succeed!! : %s"), *text);

		}
		else
		{
			OnResponseAskChatbotAnswerDelegate.ExecuteIfBound(false, TEXT(""), TEXT(""));

		}
	}
	else
	{
		// 통신 실패

		UE_LOG(LogTemp, Warning, TEXT("Req_AskByFileToChatbot Failed!!"));
	}
}