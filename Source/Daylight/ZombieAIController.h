// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "ZombieAIController.generated.h"

/**
 * 
 */
UCLASS()
class DAYLIGHT_API AZombieAIController : public AAIController
{
	GENERATED_BODY()
public:
    AZombieAIController();

protected:
    virtual void BeginPlay() override;
    virtual void OnPossess(APawn* InPawn) override;

public:
    // Behavior Tree
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
    class UBehaviorTree* BehaviorTree;

    // Blackboard
    UPROPERTY(BlueprintReadOnly, Category = "AI")
    class UBlackboardComponent* BlackboardComp;

    // Perception
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
    UAIPerceptionComponent* PerceptionComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
    UAISenseConfig_Sight* SightConfig;

    // Perception ¿Ã∫•∆Æ
    UFUNCTION()
    void OnTargetDetected(AActor* Actor, FAIStimulus Stimulus);
};
