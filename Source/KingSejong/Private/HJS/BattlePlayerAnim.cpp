// Fill out your copyright notice in the Description page of Project Settings.


#include "HJS/BattlePlayerAnim.h"
#include "HJS/HJS_BattlePlayer.h"
#include "Kismet/GameplayStatics.h"

void UBattlePlayerAnim::PlayHitMontage()
{
	check(HitMontage);
	Montage_Play(HitMontage);
}

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

void UBattlePlayerAnim::AnimNotify_AttackEnd()
{
	AHJS_BattlePlayer* Player =  Cast<AHJS_BattlePlayer>(TryGetPawnOwner());
	Player->bReturn = true;
	PlayReturnAnim();
	Montage_Stop(0.1f,AttackMontage);

}

void UBattlePlayerAnim::AnimNotify_AttackPoint()
{
	TArray<AActor*> OutActors;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(),AHJS_BattlePlayer::StaticClass(), OutActors);

	for ( AActor* Actor : OutActors )
	{
		// 이 시점에 스피어 오버랩을 뿌리지 말고, Class로 BattlePlayer를 찾아서 나 빼고 다른 사람이 맞은 사람.
		if ( Actor == GetOwningActor() )
		{
			continue;
		}

		AHJS_BattlePlayer* Player = Cast<AHJS_BattlePlayer>(Actor);
		// 그 캐릭터의 Hit 함수를 발동시키기.
		Player->PlayerHit();

		// 그리고,맞은 사람의 체력을 1 깎기.
		Player->OnMyTakeDamage(1);


	}
}

void UBattlePlayerAnim::AnimNotify_HitEnd()
{
	// Hit 애니메이션이 끝난 시점에 다음 문제 준비가 되었는 지 체크할 수 있도록 하기.

	AHJS_BattlePlayer* MainPlayer = Cast<AHJS_BattlePlayer>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	check(MainPlayer);
	MainPlayer->ServerStartTurnToGM();
	Montage_Stop(0.1f , HitMontage);
}

void UBattlePlayerAnim::PlayReturnAnim()
{
	bReturn = true;
}

void UBattlePlayerAnim::AnimNotify_ReturnEnd()
{
	bReturn = false;
}
