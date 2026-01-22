// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DaylightCharacter.h"
#include "ZombieBase.generated.h"

/**
 * 
 */
UCLASS()
class DAYLIGHT_API AZombieBase : public ADaylightCharacter
{
	GENERATED_BODY()
protected:
    virtual void BeginPlay() override;

public:
	AZombieBase();
    virtual void Tick(float DeltaTime) override;

    // AI 상태
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    float DetectionRadius = 1500.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    float AttackRange = 150.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    float AttackDamage = 10.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    float AttackCooldown = 1.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    float MoveSpeed = 300.f;
protected:
    // 현재 타겟
    UPROPERTY(BlueprintReadOnly, Category = "AI")
    AActor* CurrentTarget;

    // 공격 타이머
    float LastAttackTime;

    // AI 로직
    void UpdateAI();
    AActor* FindNearestTarget();
    void MoveToTarget();
    void AttackTarget();
    bool CanAttack() const;

    // 죽음 처리
    virtual void OnDeath_Implementation() override;
};
