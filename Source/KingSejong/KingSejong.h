// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(NetLog, Log, All);

#define LOCALROLE (UEnum::GetValueAsString<ENetRole>(GetLocalRole()))
#define REMOTEROLE (UEnum::GetValueAsString<ENetRole>(GetRemoteRole()))
#define CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))
#define NETMODE (GetWorld()->GetNetMode()==ENetMode::NM_Client?TEXT("Client") : GetWorld()->GetNetMode()==NM_Standalone?TEXT("Standalone") : TEXT("Server"))
#define PRINTLOG(fmt, ...) UE_LOG(NetLog, Warning, TEXT("[%s]%s : %s"), NETMODE, *CALLINFO , *FString::Printf(fmt, ##__VA_ARGS__))