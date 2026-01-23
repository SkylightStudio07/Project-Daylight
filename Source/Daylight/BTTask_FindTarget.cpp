// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_FindTarget.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

UBTTask_FindTarget::UBTTask_FindTarget()
{
    NodeName = "Find Target";

    // Blackboard Key 설정
    TargetActorKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_FindTarget, TargetActorKey), AActor::StaticClass());
}

EBTNodeResult::Type UBTTask_FindTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController)
        return EBTNodeResult::Failed;

    APawn* AIPawn = AIController->GetPawn();
    if (!AIPawn)
        return EBTNodeResult::Failed;

    // 플레이어 찾기
    ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

    if (Player)
    {
        float Distance = FVector::Dist(AIPawn->GetActorLocation(), Player->GetActorLocation());

        if (Distance <= SearchRadius)
        {
            // Blackboard에 저장
            OwnerComp.GetBlackboardComponent()->SetValueAsObject(TargetActorKey.SelectedKeyName, Player);
            return EBTNodeResult::Succeeded;
        }
    }

    return EBTNodeResult::Failed;
}