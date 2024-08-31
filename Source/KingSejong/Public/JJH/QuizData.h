// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "QuizData.generated.h"

/**
 * 
 */
UCLASS()
class KINGSEJONG_API UQuizData : public UObject
{
	GENERATED_BODY()
	
};

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