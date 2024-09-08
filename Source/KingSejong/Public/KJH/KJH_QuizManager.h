// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "KJH_QuizManager.generated.h"


UENUM()
enum class EQuizState : uint8
{
	Idle,
	Question,
	Waiting,
	Answer,
	End
};

USTRUCT(BlueprintType)
struct FQuizInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "QuizInfo")
	FString Quistion;

	UPROPERTY(EditAnywhere, Category = "QuizInfo")
	bool Answer;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class KINGSEJONG_API UKJH_QuizManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UKJH_QuizManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY()
	class AKJH_CommunityGameMode* CommunityGameMode;
	
	EQuizState QuizState;

	UPROPERTY()
	class AActor* OXLine;



public:
	// 퀴즈 상태별 시간
	UPROPERTY(EditDefaultsOnly)
	int32 IdleTime = 15;
	UPROPERTY(EditDefaultsOnly)
	int32 QuizTime = 30;
	UPROPERTY(EditDefaultsOnly)
	int32 WaitingTime = 5;
	UPROPERTY(EditDefaultsOnly)
	int32 AnwserTime = 10;

	// 퀴즈 개수
	UPROPERTY(EditDefaultsOnly)
	int32 MaxQuizCount = 3;
	int32 QuizCount = 0;

	UPROPERTY(EditAnywhere)
	TArray<FQuizInfo> QuizInfos;

public:
	UFUNCTION(BlueprintCallable)
	void StartQuiz();

	void SetQuizState(EQuizState State);


	// Quiz 상태
	void IdleState();
	void QuestionState();
	void WaitingState();
	void AnswerState();
	void EndState();

	bool GetPlayerQuizAnswer(class AKJH_PlayerController* TargetPC);
};
