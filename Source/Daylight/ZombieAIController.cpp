#include "ZombieAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "GameFramework/Character.h"

AZombieAIController::AZombieAIController()
{
    // Blackboard Component
    BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));

    // Behavior Tree Component (자동 생성되지만 명시적으로)
    // BehaviorTreeComponent는 AAIController에 기본 포함됨

    // Perception Component
    PerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComp"));

    // Sight Config
    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
    SightConfig->SightRadius = 1500.f;
    SightConfig->LoseSightRadius = 2000.f;
    SightConfig->PeripheralVisionAngleDegrees = 90.f;
    SightConfig->DetectionByAffiliation.bDetectEnemies = true;
    SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
    SightConfig->DetectionByAffiliation.bDetectFriendlies = false;

    PerceptionComp->ConfigureSense(*SightConfig);
    PerceptionComp->SetDominantSense(SightConfig->GetSenseImplementation());
}

void AZombieAIController::BeginPlay()
{
    Super::BeginPlay();

    // Perception 이벤트 바인딩
    if (PerceptionComp)
    {
        PerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &AZombieAIController::OnTargetDetected);
    }
}

void AZombieAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    // Behavior Tree 실행
    if (BehaviorTree && BehaviorTree->BlackboardAsset)
    {
        BlackboardComp->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
        RunBehaviorTree(BehaviorTree);

        UE_LOG(LogTemp, Log, TEXT("Zombie AI: Behavior Tree started"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Zombie AI: BehaviorTree or Blackboard not set!"));
    }
}

void AZombieAIController::OnTargetDetected(AActor* Actor, FAIStimulus Stimulus)
{
    if (!BlackboardComp) return;

    // 플레이어 감지 시 Blackboard에 저장
    if (Stimulus.WasSuccessfullySensed())
    {
        BlackboardComp->SetValueAsObject("TargetActor", Actor);
        UE_LOG(LogTemp, Log, TEXT("Zombie detected: %s"), *Actor->GetName());
    }
    else
    {
        // 시야에서 벗어남
        BlackboardComp->ClearValue("TargetActor");
        UE_LOG(LogTemp, Log, TEXT("Zombie lost sight of target"));
    }
}