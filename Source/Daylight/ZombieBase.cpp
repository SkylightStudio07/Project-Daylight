// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "WaveManager.h"
#include "DrawDebugHelpers.h"

AZombieBase::AZombieBase()
{
	PrimaryActorTick.bCanEverTick = true;
    // 좀비 설정

    LastAttackTime = -999.f;

    // 이동 속도
    GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;

    // AI Controller 자동 빙의
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AZombieBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AZombieBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    UpdateAI();
}

void AZombieBase::UpdateAI()
{
    // 타겟 찾기
    if (!CurrentTarget || !IsValid(CurrentTarget))
    {
        CurrentTarget = FindNearestTarget();
    }

    if (CurrentTarget)
    {
        float DistanceToTarget = FVector::Dist(GetActorLocation(), CurrentTarget->GetActorLocation());

        // 공격 범위 안
        if (DistanceToTarget <= AttackRange)
        {
            if (CanAttack())
            {
                AttackTarget();
            }
        }
        // 추적
        else if (DistanceToTarget <= DetectionRadius)
        {
            MoveToTarget();
        }
        // 범위 밖
        else
        {
            CurrentTarget = nullptr;
        }


    }
}

AActor* AZombieBase::FindNearestTarget()
{
    // 플레이어 찾기
    AActor* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

    if (Player)
    {
        float Distance = FVector::Dist(GetActorLocation(), Player->GetActorLocation());
        if (Distance <= DetectionRadius)
        {
            return Player;
        }
    }

    return nullptr;
}

void AZombieBase::MoveToTarget()
{
    if (!CurrentTarget) return;

    // 타겟 방향으로 이동
    FVector Direction = (CurrentTarget->GetActorLocation() - GetActorLocation()).GetSafeNormal();
    AddMovementInput(Direction, 1.0f);

    // 타겟 쳐다보기
    FRotator LookAtRotation = (CurrentTarget->GetActorLocation() - GetActorLocation()).Rotation();
    LookAtRotation.Pitch = 0.f;
    LookAtRotation.Roll = 0.f;
    SetActorRotation(FMath::RInterpTo(GetActorRotation(), LookAtRotation, GetWorld()->GetDeltaSeconds(), 5.f));
}

void AZombieBase::AttackTarget()
{
    if (!CurrentTarget || !CanAttack()) return;

    LastAttackTime = GetWorld()->GetTimeSeconds();

    // 데미지 적용
    UGameplayStatics::ApplyDamage(
        CurrentTarget,
        AttackDamage,
        GetController(),
        this,
        UDamageType::StaticClass()
    );

    UE_LOG(LogTemp, Warning, TEXT("Zombie attacked %s for %.1f damage"),
        *CurrentTarget->GetName(), AttackDamage);

    // 공격 애니메이션 재생 (나중에 추가)
    // PlayAttackAnimation();
}

bool AZombieBase::CanAttack() const
{
    float TimeSinceLastAttack = GetWorld()->GetTimeSeconds() - LastAttackTime;
    return TimeSinceLastAttack >= AttackCooldown;
}

void AZombieBase::OnDeath_Implementation()
{
    Super::OnDeath_Implementation();

    TArray<AActor*> FoundManagers;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWaveManager::StaticClass(), FoundManagers);

    if (FoundManagers.Num() > 0)
    {
        AWaveManager* Manager = Cast<AWaveManager>(FoundManagers[0]);
        if (Manager)
        {
            Manager->OnZombieKilled();
        }

        // 좀비 죽음 처리
        UE_LOG(LogTemp, Warning, TEXT("Zombie died: %s"), *GetName());

        // AI 정지
        SetActorTickEnabled(false);

        // 충돌 비활성화
        GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));

        // Ragdoll
        GetMesh()->SetSimulatePhysics(true);

        // 3초 후 제거
        FTimerHandle DestroyTimer;
        GetWorldTimerManager().SetTimer(DestroyTimer, [this]()
            {
                Destroy();
            }, 3.0f, false);
    }
}