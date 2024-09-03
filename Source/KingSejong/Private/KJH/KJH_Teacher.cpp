// Fill out your copyright notice in the Description page of Project Settings.


#include "KJH/KJH_Teacher.h"
#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "Net/UnrealNetwork.h"
#include "KJH/Widget/KJH_VoiceRecodingWidget.h"
#include "Components/WidgetComponent.h"
#include "KJH/Widget/KJH_SpeechBubbleWidget.h"

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
}

// Called when the game starts or when spawned
void AKJH_Teacher::BeginPlay()
{
    Super::BeginPlay();
    
    // CastSpeechBubbleText();
    // SetSpeechBubbleText(FString("Idle"));

    SetVisiblityStateWidget(true);

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
    DOREPLIFETIME(AKJH_Teacher, StateWidgetComp);
    DOREPLIFETIME(AKJH_Teacher, SpeechBubbleWidget);
}

void AKJH_Teacher::OnBeginInteraction(AActor* OtherActor)
{
    Super::OnBeginInteraction(OtherActor);

    SetOwner(OtherActor);

    ClientRPC_CreateRecodingWidget();
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
    SetTeacherState(ETeacherState::Idle);
}

void AKJH_Teacher::CreateRecodingWidget()
{
    // 질문 UI 열기
    // 열리는 건 동기화 안되어야 함
    RecodingWidget = Cast<UKJH_VoiceRecodingWidget>(CreateWidget(GetWorld(), RecodingWidgetFactory));

    if ( RecodingWidget )
    {
        RecodingWidget->AddToViewport(0);
        RecodingWidget->OnCloseWidgetDelegate.AddUObject(this, &AKJH_Teacher::SetTeacherStateToIdle);
        RecodingWidget->OnSuccessedAnswerDelegate.AddUObject(this, &AKJH_Teacher::SetSpeechBubbleText);

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
    ServerRPC_SetTeacherState(NewState);
}

void AKJH_Teacher::ServerRPC_SetTeacherState_Implementation(ETeacherState NewState)
{
    MulticastRPC_SetTeacherState(NewState);
}

void AKJH_Teacher::MulticastRPC_SetTeacherState_Implementation(ETeacherState NewState)
{
    TeacherState = NewState;

}

void AKJH_Teacher::SetSpeechBubbleText(FString Message)
{
    //SpeechBubbleWidget->SetTextMessage(Message);

    ServerRPC_SetSpeechBubbleText(Message);
}

void AKJH_Teacher::ServerRPC_SetSpeechBubbleText_Implementation( const FString& Message)
{
    MulticastRPC_SetSpeechBubbleText(Message);
}

void AKJH_Teacher::MulticastRPC_SetSpeechBubbleText_Implementation(const FString& Message)
{

    CastSpeechBubbleText();
    SpeechBubbleWidget->SetTextMessage(Message);

    FTimerHandle timderHandle;
    GetWorld()->GetTimerManager().SetTimer(timderHandle, this, &AKJH_Teacher::SetSpeechBubbleText_Test, 3 );
}

void AKJH_Teacher::CastSpeechBubbleText()
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

void AKJH_Teacher::SetSpeechBubbleText_Test()
{
    SpeechBubbleWidget->SetTextMessage(FString("sdfsdfsdf"));
}
