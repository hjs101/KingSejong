// Fill out your copyright notice in the Description page of Project Settings.


#include "KJH/KJH_Teacher.h"
#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Containers/StringConv.h"
#include "Net/UnrealNetwork.h"
#include "KJH/Widget/KJH_VoiceRecodingWidget.h"
#include "KJH/Widget/KJH_SpeechBubbleWidget.h"
#include "KJH/API/KJH_HttpManager.h"
#include "KJH/KJH_CommunityGameState.h"
#include "KJH/API/KJH_FileDataLib.h"



AKJH_Teacher::AKJH_Teacher()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    bReplicates = true;

    // Collision
    CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
    SetRootComponent(CapsuleComp);
    CapsuleComp->SetCapsuleHalfHeight(88);
    CapsuleComp->SetCapsuleRadius(43);

    // SkeletalMesh
    SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComp"));
    SkeletalMeshComp->SetupAttachment(RootComponent);
    SkeletalMeshComp->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));

    // WidgetComp
    StateWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("StateWidgetComp"));
    StateWidgetComp->SetupAttachment(RootComponent);

    // AudioComp
    AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComp"));
    AudioComp->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AKJH_Teacher::BeginPlay()
{
    Super::BeginPlay();

    // GameState
    MyGameState = Cast<AKJH_CommunityGameState>(GetWorld()->GetGameState());

    CastSpeechBubbleWidget();

    FTimerHandle timerHandle;
    GetWorld()->GetTimerManager().SetTimer(timerHandle,
        [ this ] ()
        {   
            OnReq_TeacherState();
        },
        0.05f, false
    );

    // HttpManager
    HttpManager = Cast<AKJH_HttpManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AKJH_HttpManager::StaticClass()));
    if ( HttpManager )
    {
        HttpManager->OnResponseAskChatbotAnswerDelegate.BindUObject(this, &AKJH_Teacher::OnRes_ChatbotResult);
        HttpManager->OnResponseGetAudioDataDelegate.BindUObject(this, &AKJH_Teacher::OnRes_ChatbotAudioData);
    }
}

// Called every frame
void AKJH_Teacher::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AKJH_Teacher::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AKJH_Teacher, TeacherState);
}

/// <summary>
/// Server에서 실행되고 있는 함수
/// </summary>
/// <param name="OtherActor"> 상호작용을 한 플레이어 </param>
void AKJH_Teacher::OnBeginInteraction(AActor* OtherActor)
{
    Super::OnBeginInteraction(OtherActor);

    if ( OtherActor == nullptr )
    {
        UE_LOG(LogTemp, Warning, TEXT("OtherActor이 없습니다."));
        return;
    }

    // SetOwner(OtherActor);
    ClientRPC_CreateRecodingWidget();

    //if( RecodingWidget )
    //    ServerRPC_SetTeacherState(ETeacherState::Listen);
}

void AKJH_Teacher::OnEndInteraction()
{
    Super::OnEndInteraction();

    // 대기 상태 변경한다.
    ServerRPC_SetTeacherState(ETeacherState::Idle);

}

bool AKJH_Teacher::IsInteractable()
{
    // 대기 상태일 때만 인터렉션 가능
    return TeacherState == ETeacherState::Idle;
}


void AKJH_Teacher::SetTeacherStateToIdle()
{
    ServerRPC_SetTeacherState(ETeacherState::Idle);
}

/// <summary>
/// Server에서 상호작용을 한 Client에게 Widget 생성을 요청하는 함수
/// - Local 함수
/// </summary>
void AKJH_Teacher::CreateRecodingWidget()
{
    // 질문 UI 열기
    // 열리는 건 동기화 안되어야 함
    RecodingWidget = Cast<UKJH_VoiceRecodingWidget>(CreateWidget(GetWorld(), RecodingWidgetFactory));

    if ( RecodingWidget )
    {
        RecodingWidget->AddToViewport(0);
        RecodingWidget->OnCloseWidgetDelegate.AddUObject(this, &AKJH_Teacher::SetTeacherStateToIdle);
        // RecodingWidget->OnResponseQuestResultDelegate.AddUObject(this, &AKJH_Teacher::OnRes_ChatbotResult);

        //서버에게 훈장님 상태를 변경 요청
        ServerRPC_SetTeacherState(ETeacherState::Listen);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Recoding Widget이 없습니다."));
    }
}

void AKJH_Teacher::ClientRPC_CreateRecodingWidget_Implementation()
{
    CreateRecodingWidget();
}

void AKJH_Teacher::SetTeacherState(ETeacherState NewState)
{
    // 훈장님 상태 변경
    TeacherState = NewState;

    //if ( HasAuthority() )
    //{
    //    TeacherState = NewState;
    //}
    //else
    //{
    //    OnReq_TeacherState();
    //}

}

void AKJH_Teacher::ServerRPC_SetTeacherState_Implementation(ETeacherState NewState)
{
    TeacherState = NewState;

    MulticastRPC_SetTeacherState(NewState);
}

void AKJH_Teacher::MulticastRPC_SetTeacherState_Implementation(ETeacherState NewState)
{
    //SetTeacherState(NewState);
    //TeacherState = NewState;
    OnReq_TeacherState();
}

void AKJH_Teacher::SetSpeechBubbleText(FString Message)
{
    CastSpeechBubbleWidget();

    SpeechBubbleWidget->SetTextMessage(Message);

    //ServerRPC_SetSpeechBubbleText(Message);
}

void AKJH_Teacher::ServerRPC_SetSpeechBubbleText_Implementation( const FString& Message)
{
    MulticastRPC_SetSpeechBubbleText(Message);
}

void AKJH_Teacher::MulticastRPC_SetSpeechBubbleText_Implementation(const FString& Message)
{
    CastSpeechBubbleWidget();
    SetSpeechBubbleText(Message);

    //// 임시 코드
    //// todo : 음성 재생 끝났을 때 idle 상태로 전환되도록 수정
    //FTimerHandle timerHandle;
    //GetWorld()->GetTimerManager().SetTimer(timerHandle,
    //    [ this ] ()
    //    {
    //        SetTeacherState(ETeacherState::Idle);
    //    },
    //    5.0f, false
    //);

}

void AKJH_Teacher::CastSpeechBubbleWidget()
{
    if ( SpeechBubbleWidget == nullptr )
    {
        SpeechBubbleWidget = Cast<UKJH_SpeechBubbleWidget>(StateWidgetComp->GetWidget());

        if ( SpeechBubbleWidget != nullptr )
        {
            if ( MyGameState == nullptr )
            {
                auto* gs = Cast<AKJH_CommunityGameState>(GetWorld()->GetGameState());

                if ( gs != nullptr )
                {
                    MyGameState = gs;

                    SpeechBubbleWidget->SetTextMessage(MyGameState->TeacherMessage);
                }
            }
            
        }
    }
}

void AKJH_Teacher::ServerRPC_GetChatbotAudioData_Implementation()
{
    MulticastRPC_GetChatbotAudioData();
}

void AKJH_Teacher::MulticastRPC_GetChatbotAudioData_Implementation()
{
    if ( HttpManager )
    {
        //HttpManager->Req_GetChatbotAudioData(TEXT("22c6d2da-ee40-4219-bb6c-95c69cfc5b63"));
        HttpManager->Req_GetChatbotAudioData(MyGameState->TeacherAudioId);
    }
}


FString AKJH_Teacher::GetMessageByTeacherState(ETeacherState NewState)
{
    FString message;
    switch ( NewState )
    {
    case ETeacherState::Idle:
        message = FString(TEXT("무엇이 궁금한고?"));
        break;
    case ETeacherState::Listen:
        message = FString(TEXT("음...기다려보거라"));
        break;
    case ETeacherState::Think:
        message = FString(TEXT("Think.."));
        break;
    case ETeacherState::Answer:
        message = FString(TEXT("Answer.."));
        break;
    default:
        break;
    }

    return message;
}

void AKJH_Teacher::OnRes_ChatbotResult(bool bResult, const FString& AudioId, const FString& Text)
{
    // 통신 실패
    if ( bResult == false || Text.IsEmpty())
    {
        OnFailedChatbotResponse();
    }
    // 통신 성공
    else
    {

        MyGameState->OnChangedTeacherMessage(Text);
        MyGameState->OnChangedTeacherAudioId(AudioId);

        OnSuccessedChatbotResponse();
    }
}

void AKJH_Teacher::OnReq_TeacherState()
{
    // 훈장님 상태 변경
    if ( TeacherState == ETeacherState::Answer )
        return;

    CastSpeechBubbleWidget();

    MyGameState->TeacherMessage = GetMessageByTeacherState(TeacherState);
    SpeechBubbleWidget->SetTextMessage(MyGameState->TeacherMessage);
}

void AKJH_Teacher::OnSuccessedChatbotResponse()
{
    FTimerHandle timerHandle;
    GetWorld()->GetTimerManager().SetTimer(timerHandle,
        [ this ] ()
        {
            ServerRPC_GetChatbotAudioData();
        },
        AudioRequestDelayTime, false
    );

    UE_LOG(LogTemp, Warning, TEXT("훈장님 text 답변 : %s"), *MyGameState->TeacherMessage);


}
void AKJH_Teacher::OnFailedChatbotResponse()
{
    ServerRPC_SetTeacherState(ETeacherState::Answer);
    ServerRPC_SetSpeechBubbleText(TEXT("질문을 이해하지 못했구나"));

    FTimerHandle timerHandle;
    GetWorld()->GetTimerManager().SetTimer(timerHandle,
        [ this ] ()
        {
            ServerRPC_SetTeacherState(ETeacherState::Idle);
        },
        IdleDelayTime, false
    );
}

void AKJH_Teacher::OnRes_ChatbotAudioData(const FString& AudioData)
{
    bool bIsSuccessed = MyGameState != nullptr && AudioData.IsEmpty() == false;
    // 훈장님 오디오 응답 통신 성공했을 경우
    if (bIsSuccessed)
    {
        // wav 파일을 변환해서 사용하던지
        // data를 디코딩해서 사용하던지해서 play하기
        // 오디오 재생
        auto* soundWave = UKJH_FileDataLib::CreateSoundWaveToAudioData(AudioData);

        if (soundWave)
        {
            AudioComp->SetSound(soundWave);
            AudioComp->Play();

            UE_LOG(LogTemp, Display, TEXT("[OnRes_ChatbotAudioData] Audio Play!!"));

        }
    }

    if (HasAuthority())
    {
        // 텍스트 변경
        ServerRPC_SetTeacherState(ETeacherState::Answer);
        ServerRPC_SetSpeechBubbleText(MyGameState->TeacherMessage);

        UE_LOG(LogTemp, Display, TEXT("[OnRes_ChatbotAudioData] TEXT : %s"), *MyGameState->TeacherMessage);

        // 일정 시간 뒤 idle 상태로 변경
        FTimerHandle timerHandle;
        GetWorld()->GetTimerManager().SetTimer(timerHandle,
            [this]()
            {
                ServerRPC_SetTeacherState(ETeacherState::Idle);

                UE_LOG(LogTemp, Display, TEXT("[OnRes_ChatbotAudioData] Idle로 상태 변경"));

            },
            IdleDelayTime, false
        );

    }

    //// 텍스트 변경
    //SetTeacherState(ETeacherState::Answer);
    //SetSpeechBubbleText(MyGameState->TeacherMessage);

    //UE_LOG(LogTemp, Display, TEXT("[OnRes_ChatbotAudioData] TEXT : %s"), *MyGameState->TeacherMessage);

    //// 일정 시간 뒤 idle 상태로 변경
    //FTimerHandle timerHandle;
    //GetWorld()->GetTimerManager().SetTimer(timerHandle,
    //    [this]()
    //    {
    //        SetTeacherState(ETeacherState::Idle);

    //        UE_LOG(LogTemp, Display, TEXT("[OnRes_ChatbotAudioData] Idle로 상태 변경"));

    //    },
    //    IdleDelayTime, false
    //);

}
