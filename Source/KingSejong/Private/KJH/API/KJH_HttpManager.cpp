// Fill out your copyright notice in the Description page of Project Settings.


#include "KJH/API/KJH_HttpManager.h"
#include "HttpModule.h"
#include "KJH/API/KJH_JsonParseLib.h"
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

	//MyGameState = Cast<AKJH_CommunityGameState>(GetWorld()->GetGameState());
}


/// <summary>
/// 파일 경로로 질문하기
/// </summary>
/// <param name="BookName"> 책 이름</param>
/// <param name="FilePath"> 보이스 파일 경로 </param>
void AKJH_HttpManager::Req_AskToChatbot(const FString& FilePath)
{
	FHttpModule& httpModule = FHttpModule::Get();
	TSharedRef<IHttpRequest> req = httpModule.CreateRequest();

	// 파일 읽기
	TArray<uint8> FileData;
	if ( !FFileHelper::LoadFileToArray(FileData, *FilePath) )
	{
		UE_LOG(LogTemp, Error, TEXT("파일 읽기 실패!: %s"), *FilePath);

		OnResponseAskChatbotAnswerDelegate.ExecuteIfBound(false, TEXT(""), TEXT(""));
		return;
	}
	// 읽어온 파일을 Base64로 인코딩
	FString EncodingFileData = FBase64::Encode(FileData);

	// 통신할 데이터
	TMap<FString, FString> data;
	data.Add(TEXT("audio"), EncodingFileData);
	req->SetContentAsString(UKJH_JsonParseLib::MakeJson(data));

	// 요청 정보
	req->SetURL(ChatbotTextServerURL);
	req->SetVerb(TEXT("POST"));
	req->SetHeader(TEXT("content-type"), TEXT("application/json"));
	req->SetContentAsString(UKJH_JsonParseLib::MakeJson(data));

	// 응답 처리
	req->OnProcessRequestComplete().BindUObject(this, &AKJH_HttpManager::OnRes_AskToChatbot);

	// 서버에 요청
	req->ProcessRequest();

	UE_LOG(LogTemp, Warning, TEXT("Req_AskToChatbot API 요청 : %s"), *ChatbotTextServerURL);

}

void AKJH_HttpManager::OnRes_AskToChatbot(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	TMap<FString, FString> result;
	if ( bConnectedSuccessfully )
	{
		FString res = Response->GetContentAsString();
		result = UKJH_JsonParseLib::JsonParseChatbotAnswer(res);

		if ( result.Contains(UKJH_JsonParseLib::TEXT_KEY) && result.Contains(UKJH_JsonParseLib::AUDIO_ID_KEY) )
		{
			FString text = result[ UKJH_JsonParseLib::TEXT_KEY ];
			FString audioId = result[ UKJH_JsonParseLib::AUDIO_ID_KEY ];

			if ( text.IsEmpty() == false && audioId.IsEmpty() == false )
			{
				OnResponseAskChatbotAnswerDelegate.ExecuteIfBound(true, audioId, text);
			}
			else
			{
				OnResponseAskChatbotAnswerDelegate.ExecuteIfBound(false, TEXT(""), TEXT(""));
			}
		}
		UE_LOG(LogTemp, Warning, TEXT("OnRes_AskToChatbot Successed!"));
	}
	else
	{
		// 통신 실패
		OnResponseAskChatbotAnswerDelegate.ExecuteIfBound(false, TEXT(""), TEXT(""));

		UE_LOG(LogTemp, Warning, TEXT("OnRes_AskToChatbot Failed!!"));
	}
}

void AKJH_HttpManager::Req_GetChatbotAudioData(const FString& AudioId)
{
	FHttpModule& httpModule = FHttpModule::Get();
	TSharedRef<IHttpRequest> req = httpModule.CreateRequest();

	// 요청 정보
	FString url = ChatbotAudioServerURL + FString::Printf(TEXT("\"%s\""), *AudioId);

	req->SetURL(url);
	req->SetVerb(TEXT("GET"));
	req->SetHeader(TEXT("content-type"), TEXT("application/json"));

	// 응답 처리
	req->OnProcessRequestComplete().BindUObject(this, &AKJH_HttpManager::OnRes_GetChatbotAudioData);

	// 서버에 요청
	req->ProcessRequest();

	UE_LOG(LogTemp, Warning, TEXT("GetChatbotAudio Request api : %s"), *url);

}

void AKJH_HttpManager::OnRes_GetChatbotAudioData(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	FString result;
	if (bConnectedSuccessfully)
	{
		FString res = Response->GetContentAsString();
		result = UKJH_JsonParseLib::JsonParseChatbotAudioData(res);
	}

	OnResponseGetAudioDataDelegate.ExecuteIfBound(result);
}

/// <summary>
/// 퀴즈 정보 가져오기
/// </summary>
/// <param name="BookName"></param>
void AKJH_HttpManager::Req_GetChatbotQuizData(const FString& BookName)
{
	FHttpModule& httpModule = FHttpModule::Get();
	TSharedRef<IHttpRequest> req = httpModule.CreateRequest();

	// 통신할 데이터
	TMap<FString, FString> data;
	data.Add(TEXT("query"), FString::Printf(TEXT("%s 이야기에서 빈도수가 높고, 중요한 단어와 빈도수가 높고, 중요한 문장 그리고 빈도수가 높고, 중요한 내용을 기반으로 OX퀴즈 내줘"), *BookName));
	req->SetContentAsString(UKJH_JsonParseLib::MakeJson(data));

	// 요청 정보
	req->SetURL(QuizServerURL);
	req->SetVerb(TEXT("POST"));
	req->SetHeader(TEXT("content-type"), TEXT("application/json"));
	req->SetContentAsString(UKJH_JsonParseLib::MakeJson(data));



	// 응답 처리
	req->OnProcessRequestComplete().BindUObject(this, &AKJH_HttpManager::OnRes_GetChatbotQuizData);

	// 서버에 요청
	req->ProcessRequest();

	UE_LOG(LogTemp, Warning, TEXT("Call Req_GetQuizInfo!!"));
}

void AKJH_HttpManager::OnRes_GetChatbotQuizData(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	TArray<FChatbotQuizData> result;
	if (bConnectedSuccessfully)
	{
		FString res = Response->GetContentAsString();
		result = UKJH_JsonParseLib::JsonParseChatbotQuizData(res);
	}

	OnResponseChatbotQuizDataDelegate.Broadcast(result);
}
