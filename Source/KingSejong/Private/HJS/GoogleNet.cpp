// Fill out your copyright notice in the Description page of Project Settings.


#include "HJS/GoogleNet.h"
#include "Json.h"
#include "JsonUtilities.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"

// Sets default values for this component's properties
UGoogleNet::UGoogleNet()
{

}


// Called when the game starts
void UGoogleNet::BeginPlay()
{
	Super::BeginPlay();

	// ...
}
void UGoogleNet::AnonymousLogin()
{
    FString FirebaseAPIKey = "AIzaSyBhjnLtEzw_ty5pMIukLccaK_Baa-7jOqY";
    FString URL = "https://identitytoolkit.googleapis.com/v1/accounts:signUp?key=" + FirebaseAPIKey;

    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
    Request->SetURL(URL);
    Request->SetVerb("POST");
    Request->SetHeader("Content-Type", "application/json");

    FString RequestBody = "{\"returnSecureToken\":true}";
    Request->SetContentAsString(RequestBody);

    Request->OnProcessRequestComplete().BindUObject(this, &UGoogleNet::OnAnonymousLoginComplete);
    Request->ProcessRequest();
}
void UGoogleNet::OnAnonymousLoginComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
    if (bWasSuccessful && Response->GetResponseCode() == 200)
    {
        FString JsonResponse = Response->GetContentAsString();
        TSharedPtr<FJsonObject> JsonObject;
        TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonResponse);

        if (FJsonSerializer::Deserialize(Reader, JsonObject))
        {
            FString IDToken = JsonObject->GetStringField("idToken");
            
            // 이 ID 토큰을 저장하고 이후 Firebase Storage 요청에 사용합니다.
            AnonymousID = IDToken;
        }
    }
    else
    {
        // 로그인 실패 처리
    }
}

void UGoogleNet::FileUploadToFirebase(const FString& FilePath, const FString& FileName)
{
    {
        // 파일 읽기
        TArray<uint8> FileData;
        if (!FFileHelper::LoadFileToArray(FileData, *FilePath))
        {
            UE_LOG(LogTemp, Error, TEXT("파일 읽기 실패!: %s"), *FilePath);
            return;
        }

        // HTTP 요청 생성
        TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
        HttpRequest->SetURL(FirebaseStorageUrl + FileName + "?alt=media");
        HttpRequest->SetVerb("POST");
        HttpRequest->SetHeader("Content-Type", "application/octet-stream");
        HttpRequest->SetHeader("Authorization", "Bearer " + AnonymousID); // 익명 접근의 경우 필요하지 않을 수 있음
        HttpRequest->SetContent(FileData);

        // HTTP 응답 처리
        HttpRequest->OnProcessRequestComplete().BindLambda([](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
            {
                if (bWasSuccessful && Response->GetResponseCode() == 200)
                {
                    UE_LOG(LogTemp, Log, TEXT("File uploaded successfully"));
                }
                else
                {
                    UE_LOG(LogTemp, Error, TEXT("File upload failed: %s"), *Response->GetContentAsString());
                }
            });

        // 요청 전송
        HttpRequest->ProcessRequest();
    }
}

void UGoogleNet::FileDownloadFromFirebase(const FString& SavePath, const FString& FileName)
{
    // HTTP 요청 생성
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
    HttpRequest->SetURL(FirebaseStorageUrl + FileName + "?alt=media");
    HttpRequest->SetVerb("GET");

    // HTTP 응답 처리
    HttpRequest->OnProcessRequestComplete().BindLambda([SavePath](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
        {
            if (bWasSuccessful && Response->GetResponseCode() == 200)
            {
                // 파일 저장
                if (FFileHelper::SaveArrayToFile(Response->GetContent(), *SavePath))
                {
                    UE_LOG(LogTemp, Log, TEXT("File downloaded successfully"));
                }
                else
                {
                    UE_LOG(LogTemp, Error, TEXT("Failed to save file: %s"), *SavePath);
                }
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("File download failed: %s"), *Response->GetContentAsString());
            }
        });

    // 요청 전송
    HttpRequest->ProcessRequest();
}
