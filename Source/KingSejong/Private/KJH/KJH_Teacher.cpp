// Fill out your copyright notice in the Description page of Project Settings.


#include "KJH/KJH_Teacher.h"
#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "Net/UnrealNetwork.h"

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


}

// Called when the game starts or when spawned
void AKJH_Teacher::BeginPlay()
{
    Super::BeginPlay();

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

void AKJH_Teacher::OnBeginInteraction(AActor* OtherActor)
{
    Super::OnBeginInteraction(OtherActor);

    SetOwner(OtherActor);

    // 질문 UI 열기
    // 열리는 건 동기화 안되어야 함
    RecodingWidget = CreateWidget(GetWorld(), RecodingWidgetFactory);

    if ( RecodingWidget )
    {
        RecodingWidget->AddToViewport(0);
    }

    ServerRPCSetTeacherState(ETeacherState::Listen);
    //SetTeacherState(ETeacherState::Listen);
}

void AKJH_Teacher::OnEndInteraction()
{
    Super::OnEndInteraction();

    // 대기 상태 변경한다.
    ServerRPCSetTeacherState(ETeacherState::Idle);

    // todo : 위젯 이벤트와 연결해야 함

}

bool AKJH_Teacher::IsInteractable()
{
    // 대기 상태일 때만 인터렉션 가능
    return TeacherState == ETeacherState::Idle;
}

void AKJH_Teacher::SetTeacherState(ETeacherState NewState)
{  
    TeacherState = NewState;
}


void AKJH_Teacher::ServerRPCSetTeacherState_Implementation(ETeacherState NewState)
{
    SetTeacherState(NewState);
}
