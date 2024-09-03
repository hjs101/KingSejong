// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VoiceReceiver.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class KINGSEJONG_API UVoiceReceiver : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UVoiceReceiver();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	TArray<uint8> ReceivedVoiceData;
	int32 TotalChunks;       // 예상되는 총 청크 수
	int32 ReceivedChunks;    // 현재까지 받은 청크 수
	int32 TotalDataSize;     // 음성 데이터의 전체 크기
	int32 ChunkSize;         // 청크 크기

	void InitData(int32 InTotalDataSize, int32 InChunkSize)
	{
		ReceivedVoiceData.Empty();
		TotalDataSize = InTotalDataSize;
		ChunkSize = InChunkSize;
		TotalChunks = FMath::CeilToInt((float)TotalDataSize / ChunkSize);
		ReceivedChunks = 0;
	}

	void AddChunk(const TArray<uint8>& Chunk)
	{
		ReceivedVoiceData.Append(Chunk);
		ReceivedChunks++;
	}

	bool IsComplete() const
	{
		return ReceivedChunks >= TotalChunks;
	}

	const TArray<uint8>& GetCompleteVoiceData() const
	{
		return ReceivedVoiceData;
	}
};
