// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LobbyTrigger.generated.h"

UCLASS()
class KINGSEJONG_API ALobbyTrigger : public AActor
{
	GENERATED_BODY()
	
public:	

	// Sets default values for this actor's properties
	ALobbyTrigger();

	UPROPERTY(EditAnywhere)
	class UBoxComponent* BoxCollision;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ULobbyWidget> LobbyWidgetFactory;

	UPROPERTY(EditAnywhere)
	FString TriggerCategory;	
	
	UPROPERTY(EditAnywhere)
	FString TriggerText;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION()
	void ComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
