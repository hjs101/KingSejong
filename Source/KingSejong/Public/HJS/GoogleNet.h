// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HttpFwd.h"
#include "GoogleNet.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class KINGSEJONG_API UGoogleNet : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGoogleNet();
	UFUNCTION()
	void AnonymousLogin();
	UFUNCTION()
	void FileUploadToFirebase(const FString& FilePath, const FString& FileName);
	UFUNCTION()
	void FileDownloadFromFirebase(const FString& SavePath, const FString& FileName);
	class AHJS_BattlePlayer* Me;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	
	FString AnonymousID = "";

	FString FirebaseStorageUrl = "https://firebasestorage.googleapis.com/v0/b/wavupdownload.appspot.com/o/";
	void OnAnonymousLoginComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);



};
 