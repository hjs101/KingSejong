// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "LevenshteinLib.generated.h"

/**
 * 
 */
UCLASS()
class KINGSEJONG_API ULevenshteinLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	static int32 CalculateLevenshteinDist(const FString& Orignal, const FString& Compare);

};
