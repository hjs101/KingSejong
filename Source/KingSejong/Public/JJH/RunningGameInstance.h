// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "RunningGameInstance.generated.h"

USTRUCT(BlueprintType)
struct FWordsData : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Word;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Meaning;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Initials;
};



/**
 * 
 */
UCLASS()
class KINGSEJONG_API URunningGameInstance : public UGameInstance
{
	GENERATED_BODY()

	URunningGameInstance();
public:
	FWordsData* GetWordsData(int32 Level);

private:
	UPROPERTY()
	class UDataTable* MyWordsTable;
};
