// Fill out your copyright notice in the Description page of Project Settings.


#include "HJS/HJS_BattleGameMode.h"
#include "HJS/HJS_BattlePlayer.h"
#include "GameFramework/PlayerState.h"
#include "HJS/BattleQuestionStruct.h"

AHJS_BattleGameMode::AHJS_BattleGameMode()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AHJS_BattleGameMode::BeginPlay()
{
    Super::BeginPlay();
    //GetWorldTimerManager().SetTimer(TestHandle,this,&AHJS_BattleGameMode::StartRecordingForAllPlayers,3.f,false);
}

void AHJS_BattleGameMode::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
}

void AHJS_BattleGameMode::StartRecordingForAllPlayers()
{
    // 게임에서 현재 존재하는 모든 플레이어 컨트롤러를 가져옵니다.
    int32 index = 0;
    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        // 플레이어 컨트롤러를 가져옵니다.
        APlayerController* PlayerController = It->Get();
        if (PlayerController)
        {
            // 플레이어 캐릭터를 가져옵니다.
            AHJS_BattlePlayer* BattlePlayer = Cast<AHJS_BattlePlayer>(PlayerController->GetCharacter());
            if (BattlePlayer)
            {
                // 플레이어의 녹음을 시작합니다.
                Client_StartRecording(PlayerController,index++);
            }
        }
    }
}

// 클라이언트에 녹음을 시키기
void AHJS_BattleGameMode::Client_StartRecording(APlayerController* PlayerController, int32 Index)
{
    AHJS_BattlePlayer* Character = Cast<AHJS_BattlePlayer>(PlayerController->GetCharacter());
    if (Character)
    {
        FString PlayerID = FString::Printf(TEXT("%d%d"), FMath::RandRange(10000000, 99999999),Index);
        Character->bWin = false;
        Character->StartRecording(PlayerID);
    }
}

void AHJS_BattleGameMode::SettingPlayerAnswer(const FString& Result, APlayerController* PC)
{
    FString PCName = PC->GetName();
    int32 PlayerNum = PCName[PCName.Len() - 1] - '0';
    if (PlayerNum == 0)
    {
        Player0Result = Result;
        PC0 = PC;
    }
    else
    {
        Player1Result = Result;
        PC1 = PC;
    }

    // 두 정답이 모두 들어왔다면
    if (Player0Result != "" && Player1Result != "")
    {
        // 정확도 분석 알고리즘을 통해 분석하기

        // 분석에 따른 승자 결정
        WinnerNum = CompareString();
        BattleResult();
        // 승자의 공격 애니메이션 재생
        Player0Result = "";
        Player1Result = "";
    }

}

int32 AHJS_BattleGameMode::CompareString()
{

    // 일단 1 리턴 ( 클라이언트를 승자로 )
    return 0;
}

void AHJS_BattleGameMode::BattleResult()
{
    if (WinnerNum == 0)
    {
        AHJS_BattlePlayer* Player = Cast<AHJS_BattlePlayer>(PC0->GetCharacter());
        Player->ServerWinnerSet();
        Player->UploadWavToFirebase();
        Player->SetWinnerNum(WinnerNum);
    }
    else if(WinnerNum == 1)
    {
        AHJS_BattlePlayer* Player = Cast<AHJS_BattlePlayer>(PC1->GetCharacter());
        Player->ServerWinnerSet();
        Player->UploadWavToFirebase();
        Player->SetWinnerNum(WinnerNum);
    }
    
}

void AHJS_BattleGameMode::BattleAnimPlay()
{
    if ( WinnerNum == 0 )
    {
        Player0->Attack();
    }
    else if ( WinnerNum == 1 )
    {
        Player1->Attack();
    }
}

void AHJS_BattleGameMode::JoinPlayer(APlayerController* PC)
{
    check(PC);
    FString PCName = PC->GetName();
    int32 PlayerNum = PCName[PCName.Len() - 1] - '0';

    if ( PlayerNum == 0 )
    {
        PC0 = PC;
    }
    else
    {
        PC1 = PC;
    }

    if ( PC0 != nullptr && PC1 != nullptr )
    {
        // 게임 스타트
        
        Player0 = Cast<AHJS_BattlePlayer>(PC0->GetCharacter());
        Player0->AddMainUI();
        Player0->ClientPlayTutorialUI();
        Player1 = Cast<AHJS_BattlePlayer>(PC1->GetCharacter());
        Player1->AddMainUI();
        Player1->ClientPlayTutorialUI();
    }

}

void AHJS_BattleGameMode::StartTurn()
{
    if ( ReadyPlayer == 2 )
    {
        ReadyPlayer = 1;
        return;
    }

    // 모두 준비가 되었다고 판단하고 시작한다.
    if ( ReadyPlayer == 1 )
    {
        ReadyPlayer = 2;
        // 3. 서버는 모든 플레이어의 준비상태를 확인하고, 모두 준비가 됐으면 시작 신호를 보냄 (클라이언트로)
        Player0->StartTurn();
        Player1->StartTurn();
    }
}

void AHJS_BattleGameMode::QuestionSetting()
{
    if ( ReadyPlayer == 2 )
    {
        ReadyPlayer = 1;
        return;
    }

    // 모두 준비가 되었다고 판단하고 시작한다.
    if ( ReadyPlayer == 1 )
    {
        ReadyPlayer = 2;
    
		check(QuestionDataTable);
		// 데이터 테이블의 모든 행을 가져옵니다.

		int32 RandomNum = FMath::RandRange(1 , 20);
		FString ContextString;
		FBattleQuestionData* QuestionData = QuestionDataTable->FindRow<FBattleQuestionData>(FName(FString::FromInt(RandomNum)) , ContextString);
        Player0->ClientQuestionSetting(*QuestionData);
        Player1->ClientQuestionSetting(*QuestionData);
    }
}
