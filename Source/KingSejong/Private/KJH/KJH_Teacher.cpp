// Fill out your copyright notice in the Description page of Project Settings.


#include "KJH/KJH_Teacher.h"
#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "Net/UnrealNetwork.h"
#include "KJH/Widget/KJH_VoiceRecodingWidget.h"
#include "Components/WidgetComponent.h"
#include "KJH/Widget/KJH_SpeechBubbleWidget.h"
#include "KJH/API/KJH_HttpHandler.h"
#include "Kismet/GameplayStatics.h"
#include "KJH/API/KJH_HttpManager.h"
#include "Containers/StringConv.h"

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

    //// HttpHandler
    //HttpHandler = CreateDefaultSubobject<UKJH_HttpHandler>(TEXT("HttpHandler"));
}

// Called when the game starts or when spawned
void AKJH_Teacher::BeginPlay()
{
    Super::BeginPlay();
    
    // SetSpeechBubbleText(FString("Idle"));

    SetVisiblityStateWidget(true);
    CastSpeechBubbleWidget();
    

    FTimerHandle timerHandle;
    GetWorld()->GetTimerManager().SetTimer(timerHandle,
        [ this ] ()
        {
            UE_LOG(LogTemp, Warning, TEXT("begin play에서 TeacherState : %d"), TeacherState);
            
            OnReq_TeacherState();
            //ServerRPC_SetTeacherState(TeacherState);
            //SetTeacherState(TeacherState);
        },
        0.05f, false
    );

    // HttpManager
    HttpManager = Cast<AKJH_HttpManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AKJH_HttpManager::StaticClass()));

    if ( HttpManager )
    {
        HttpManager->OnResponseAskChatbotAnswerDelegate.BindUObject(this, &AKJH_Teacher::OnRes_ChatbotResult);
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
    //DOREPLIFETIME(AKJH_Teacher, StateWidgetComp);
    //DOREPLIFETIME(AKJH_Teacher, SpeechBubbleWidget);
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

    if( RecodingWidget )
        ServerRPC_SetTeacherState(ETeacherState::Listen);
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
    //SetTeacherState(ETeacherState::Idle);
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

void AKJH_Teacher::SetVisiblityStateWidget(bool bValue)
{
    
    StateWidgetComp->SetVisibility( bValue);
}

void AKJH_Teacher::SetTeacherState(ETeacherState NewState)
{
    // 훈장님 상태 변경
    TeacherState = NewState;

    OnReq_TeacherState();

}

void AKJH_Teacher::ServerRPC_SetTeacherState_Implementation(ETeacherState NewState)
{
    MulticastRPC_SetTeacherState(NewState);
}

void AKJH_Teacher::MulticastRPC_SetTeacherState_Implementation(ETeacherState NewState)
{
    SetTeacherState(NewState);
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

    // 임시 코드
    FTimerHandle timerHandle;
    GetWorld()->GetTimerManager().SetTimer(timerHandle,
        [ this ] ()
        {
            SetTeacherState(ETeacherState::Idle);
        },
        5.0f, false
    );

}

void AKJH_Teacher::CastSpeechBubbleWidget()
{
    if ( SpeechBubbleWidget == nullptr )
    {
        SpeechBubbleWidget = Cast<UKJH_SpeechBubbleWidget>(StateWidgetComp->GetWidget());

        if ( SpeechBubbleWidget == nullptr )
        {
            if ( HasAuthority() )
            {
                UE_LOG(LogTemp, Warning, TEXT("Server : SpeechBubbleWidget null!!"));

            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Client : SpeechBubbleWidget null!!"));

            }
        }
        else
        {
            if ( HasAuthority() )
            {
                UE_LOG(LogTemp, Warning, TEXT("Server : SpeechBubbleWidget not null!!"));

            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Client : SpeechBubbleWidget not null!!"));

            }

        }

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


void AKJH_Teacher::OnRes_ChatbotResult(bool bResult, const FString& Audio, const FString& Text)
{
    // 통신 실패
    FString message;

    if ( bResult == false || Text.IsEmpty())
    {
        message = FString(TEXT("훈장님과의 소통이 원활하지 않습니다."));
    }
    // 통신 성공
    else
    {
        message = Text;

    }

w    ServerRPC_SetTeacherState(ETeacherState::Answer);
    ServerRPC_SetSpeechBubbleText(message);

    // todo : 녹음 파일 재생하도록 변경
    // 녹음 파일 재생이 끝나면 훈장님 상태를 Idle로 변경해야 함

    

}

void AKJH_Teacher::OnReq_TeacherState()
{
    // 훈장님 상태 변경

    if ( TeacherState != ETeacherState::Answer )
    {
        CastSpeechBubbleWidget();

        FString message = GetMessageByTeacherState(TeacherState);
        SpeechBubbleWidget->SetTextMessage(message);
    }
}
