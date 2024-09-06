// Fill out your copyright notice in the Description page of Project Settings.


#include "JJH/RunningGameInstance.h"

URunningGameInstance::URunningGameInstance()
{
	FString WordsDataPath = TEXT("/Script/Engine.DataTable'/Game/JJH/WordsDataCSV.WordsDataCSV'");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_Words(*WordsDataPath);
	if (DT_Words.Succeeded())
	{
		MyWordsTable = DT_Words.Object;
	}
}
