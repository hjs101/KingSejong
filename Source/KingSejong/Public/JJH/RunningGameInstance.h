// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "JJH/QuizData.h"
#include "RunningGameInstance.generated.h"


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
