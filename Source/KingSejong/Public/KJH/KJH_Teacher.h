// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KJH/KJH_InteractiveActor.h"
#include "KJH_Teacher.generated.h"


UENUM()
enum class ETeacherState : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	Listen UMETA(DisplayName = "Listen"),
	Think UMETA(DisplayName = "Think"),
	Answer UMETA(DisplayName = "Answer")
};

UCLASS()
class KINGSEJONG_API AKJH_Teacher : public AKJH_InteractiveActor
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	AKJH_Teacher();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void OnBeginInteraction(class AActor* OtherActor) override;
	virtual void OnEndInteraction() override;
	virtual bool IsInteractable() override;;

public:

	// 현재 상태
	UPROPERTY(ReplicatedUsing = OnReq_TeacherState)
	ETeacherState TeacherState = ETeacherState::Idle;

	//UPROPERTY(Replicated)
	//FString TeacherMessage;
	//UPROPERTY(Replicated)
	//FString TeacherAudioId;
	
	UPROPERTY(EditDefaultsOnly)
	float AnswerDelayTime = 5;

	// Component
	UPROPERTY(EditDefaultsOnly)
	class UCapsuleComponent* CapsuleComp;
	UPROPERTY(EditDefaultsOnly)
	class USkeletalMeshComponent* SkeletalMeshComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UAudioComponent* AudioComp;

	// UI
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> RecodingWidgetFactory;
	UPROPERTY()
	class UKJH_VoiceRecodingWidget* RecodingWidget;
	UPROPERTY(EditDefaultsOnly)
	class UWidgetComponent* StateWidgetComp;
	UPROPERTY()
	class UKJH_SpeechBubbleWidget* SpeechBubbleWidget;

	// API
	UPROPERTY()
	class AKJH_HttpManager* HttpManager;



private:
	class AKJH_CommunityGameState* MyGameState;

public:

	UFUNCTION(BlueprintCallable)
	void SetTeacherState(ETeacherState NewState);
	void SetTeacherStateToIdle();


private:
		
	void CreateRecodingWidget();


		// 훈장님 상태 변경 이벤트 함수
	UFUNCTION()
	void OnReq_TeacherState();

	/* 질문하기 */
    UFUNCTION(Client, Reliable)
    void ClientRPC_CreateRecodingWidget();

	UFUNCTION(Server, Reliable)
	void ServerRPC_SetTeacherState(ETeacherState NewState);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_SetTeacherState(ETeacherState NewState);

	/* 말풍선 */
	void CastSpeechBubbleWidget();
	
	void SetSpeechBubbleText(FString Message);

	UFUNCTION(Server, Reliable)
	void ServerRPC_SetSpeechBubbleText(const FString& Message);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_SetSpeechBubbleText(const FString& Message);

	/* 음성 데이터 가져오기*/
	void GetChatbotAudioData();

	UFUNCTION(Server, Reliable)
	void ServerRPC_GetChatbotAudioData();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_GetChatbotAudioData();
	
	FString GetMessageByTeacherState(ETeacherState NewState);

	// 훈장님 음성
	void OnRes_ChatbotResult(bool bResult, const FString& AudioId, const FString& Text);
	void OnRes_ChatbotAudioData(const FString& AudioData);

	void OnSuccessedChatbotResponse();
	void OnFailedChatbotResponse();

};
