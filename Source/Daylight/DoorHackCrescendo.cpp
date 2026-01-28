// Fill out your copyright notice in the Description page of Project Settings.

#include "DoorHackCrescendo.h"
#include "Interactable.h"  // <- 이렇게 include만!
#include "ZombieSpawnVolume.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

ADoorHackCrescendo::ADoorHackCrescendo()
{
    PrimaryActorTick.bCanEverTick = false;

    // Root
    USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    RootComponent = Root;

    // 터미널 메시 (해킹할 컴퓨터/패널)
    TerminalMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TerminalMesh"));
    TerminalMesh->SetupAttachment(RootComponent);

    // 상호작용 영역
    InteractionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionBox"));
    InteractionBox->SetupAttachment(RootComponent);
    InteractionBox->SetBoxExtent(FVector(100.f, 100.f, 100.f));
}

void ADoorHackCrescendo::BeginPlay()
{
    Super::BeginPlay();
}

void ADoorHackCrescendo::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ADoorHackCrescendo::StartHack(AActor* HackInstigator)
{
    if (bIsHacking || bIsCompleted)
    {
        UE_LOG(LogTemp, Warning, TEXT("Hack already in progress or completed"));
        return;
    }

    bIsHacking = true;
    HackProgress = 0.f;

    UE_LOG(LogTemp, Warning, TEXT("=== HACK STARTED - ALARM TRIGGERED! ==="));

    // 경보음
    if (AlarmSound)
    {
        UGameplayStatics::PlaySound2D(GetWorld(), AlarmSound);
    }

    // 좀비 스폰 시작
    StartSpawning();

    // 진행도 업데이트 (0.1초마다)
    GetWorldTimerManager().SetTimer(
        ProgressTimerHandle,
        this,
        &ADoorHackCrescendo::UpdateProgress,
        0.1f,
        true
    );

    // 해킹 완료 타이머
    GetWorldTimerManager().SetTimer(
        HackTimerHandle,
        this,
        &ADoorHackCrescendo::CompleteHack,
        HackDuration,
        false
    );

    OnHackStarted();
}

void ADoorHackCrescendo::UpdateProgress()
{
    if (!bIsHacking)
    {
        GetWorldTimerManager().ClearTimer(ProgressTimerHandle);
        return;
    }

    // 남은 시간 계산
    float Remaining = GetWorldTimerManager().GetTimerRemaining(HackTimerHandle);
    HackProgress = 1.0f - (Remaining / HackDuration);
    HackProgress = FMath::Clamp(HackProgress, 0.f, 1.f);

    OnHackProgress(HackProgress);
}

void ADoorHackCrescendo::CompleteHack()
{
    if (!bIsHacking)
        return;

    bIsHacking = false;
    bIsCompleted = true;
    HackProgress = 1.f;

    GetWorldTimerManager().ClearTimer(ProgressTimerHandle);
    GetWorldTimerManager().ClearTimer(SpawnTimerHandle);

    UE_LOG(LogTemp, Warning, TEXT("=== HACK COMPLETE! ==="));

    // 완료 사운드
    if (HackCompleteSound)
    {
        UGameplayStatics::PlaySound2D(GetWorld(), HackCompleteSound);
    }

    // 문 열기
    OpenDoor();

    OnHackCompleted();
}

void ADoorHackCrescendo::OpenDoor()
{
    if (DoorActor)
    {
        UE_LOG(LogTemp, Warning, TEXT("Door opening: %s"), *DoorActor->GetName());

        // Blueprint에서 문 열기 구현
        // 예: 애니메이션, 이동 등
    }
}

void ADoorHackCrescendo::StartSpawning()
{
    if (SpawnVolumes.Num() == 0)
    {
        UE_LOG(LogTemp, Error, TEXT("No spawn volumes assigned!"));
        return;
    }

    SpawnedCount = 0;
    KilledCount = 0;

    GetWorldTimerManager().SetTimer(
        SpawnTimerHandle,
        this,
        &ADoorHackCrescendo::SpawnNextZombie,
        SpawnInterval,
        true,
        0.f  // 즉시 시작
    );
}

void ADoorHackCrescendo::SpawnNextZombie()
{
    if (SpawnedCount >= ZombieCount)
    {
        GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
        UE_LOG(LogTemp, Log, TEXT("All zombies spawned: %d"), SpawnedCount);
        return;
    }

    // 랜덤 스폰 볼륨
    int32 RandomIndex = FMath::RandRange(0, SpawnVolumes.Num() - 1);
    AZombieSpawnVolume* SpawnVolume = SpawnVolumes[RandomIndex];

    if (SpawnVolume)
    {
        SpawnVolume->SpawnZombie();
        SpawnedCount++;
    }
}

void ADoorHackCrescendo::OnZombieKilled()
{
    KilledCount++;

    UE_LOG(LogTemp, Log, TEXT("Zombie killed: %d/%d"), KilledCount, ZombieCount);
}

// IInteractable 인터페이스 구현
bool ADoorHackCrescendo::CanInteract_Implementation(AActor* InteractInstigator) const
{
    return !bIsHacking && !bIsCompleted;
}

void ADoorHackCrescendo::Interact_Implementation(AActor* InteractInstigator)
{
    StartHack(InteractInstigator);
}

FString ADoorHackCrescendo::GetInteractionPrompt_Implementation() const
{
    if (bIsCompleted)
        return TEXT("Door Unlocked");

    if (bIsHacking)
        return FString::Printf(TEXT("Hacking... %.0f%%"), HackProgress * 100.f);

    return TEXT("E - Hack Door");
}