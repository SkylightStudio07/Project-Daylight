#include "BTTask_Attack.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

UBTTask_Attack::UBTTask_Attack()
{
    NodeName = "Attack Target";
    bNotifyTick = true;  // Tick 활성화

    TargetActorKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_Attack, TargetActorKey), AActor::StaticClass());
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController)
        return EBTNodeResult::Failed;

    APawn* AIPawn = AIController->GetPawn();
    if (!AIPawn)
        return EBTNodeResult::Failed;

    // 타겟 가져오기
    AActor* Target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetActorKey.SelectedKeyName));
    if (!Target)
        return EBTNodeResult::Failed;

    // 거리 체크
    float Distance = FVector::Dist(AIPawn->GetActorLocation(), Target->GetActorLocation());

    if (Distance <= AttackRange)
    {
        // 데미지 적용
        UGameplayStatics::ApplyDamage(
            Target,
            AttackDamage,
            AIController,
            AIPawn,
            UDamageType::StaticClass()
        );

        UE_LOG(LogTemp, Warning, TEXT("Zombie dealt %.1f damage to %s"), AttackDamage, *Target->GetName());

        // 공격 후 대기
        return EBTNodeResult::InProgress;  // 진행 중 상태로 변경
    }

    return EBTNodeResult::Failed;
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

    // 쿨다운 체크 (간단 버전)
    static float Timer = 0.f;
    Timer += DeltaSeconds;

    if (Timer >= 1.5f)  // AttackCooldown
    {
        Timer = 0.f;
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    }
}