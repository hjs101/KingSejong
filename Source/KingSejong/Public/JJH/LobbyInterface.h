// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "LobbyInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class ULobbyInterface : public UInterface
{
    GENERATED_BODY()
};

/**
 * 
 */
class KINGSEJONG_API ILobbyInterface
{
    GENERATED_BODY()

public:
    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
    virtual void CreateSession(const FString& RoomName, int32 PlayerCount) = 0;

    virtual void JoinToSession(int32 Index) = 0;
};