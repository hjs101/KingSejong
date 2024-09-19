// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BattleQuestionStruct.generated.h"

/**
 * 
 */
UCLASS()
class KINGSEJONG_API UBattleQuestionStruct : public UObject
{
	GENERATED_BODY()
};

USTRUCT(Atomic, BlueprintType)
struct KINGSEJONG_API FBattleQuestionData : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Question")
	FString Question;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Question")
	FString Answer1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Question")
	FString Answer2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Question")
	FString Answer3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Question")
	FString Answer4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Question")
	FString Answer;
};
 