// Fill out your copyright notice in the Description page of Project Settings.


#include "HJS/BattlePlayerAnim.h"

void UBattlePlayerAnim::PlayChargingMontage()
{
	bAttack = true;
}

void UBattlePlayerAnim::JumpMontageToAttack()
{
	Montage_JumpToSection(TEXT("Attack"),AttackMontage);
}

void UBattlePlayerAnim::AnimNotify_ToCharging()
{
	bAttack = false;
	Montage_Play(AttackMontage);
}
