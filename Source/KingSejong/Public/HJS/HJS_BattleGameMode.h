// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HJS_BattleGameMode.generated.h"

/**
 * 
 */
UCLASS()
class KINGSEJONG_API AHJS_BattleGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	// Sets default values for this character's properties
	AHJS_BattleGameMode();

protected:
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;
public:
	void StartRecordingForAllPlayers();

	FTimerHandle TestHandle;

	// 클라이언트에서 실행되는 함수
    UFUNCTION()
    void Client_StartRecording(APlayerController* PlayerController, int32 Index);

	FString AnswerString = "";

	FString Player0Result = "";
	UPROPERTY()
	APlayerController* PC0;

	FString Player1Result = "";
	UPROPERTY()
	APlayerController* PC1;

	UPROPERTY()
	class AHJS_BattlePlayer* Player0;

	UPROPERTY()
	class AHJS_BattlePlayer* Player1;

	UFUNCTION()
	void SettingPlayerAnswer(const FString& Result, APlayerController* PC);

	UFUNCTION()
	int32 CompareString(int32 Player0Score, int32 Player1Score);

	UFUNCTION()
	void BattleResult(const FString& WinnerText);

	int32 WinnerNum = -1;

	UFUNCTION()
	void BattleAnimPlay();

	UFUNCTION()
	void JoinPlayer(APlayerController* PC);

	int32 ReadyPlayer = 2;

	UFUNCTION()
	void StartTurn();

	UPROPERTY(EditAnywhere, Category = "Question")
	class UDataTable* QuestionDataTable;

	UFUNCTION()
	void QuestionSetting();

	FString CurrentString = TEXT("");

};
