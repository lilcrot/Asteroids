// A test task by KEFIR
#include "AI/BTServices/Service_UpdateBaseEnemyBBInfo.h"
#include "AI/BaseSpacecraftEnemy.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UService_UpdateBaseEnemyBBInfo::UService_UpdateBaseEnemyBBInfo()
{
    Interval = 0.5f;
}

void UService_UpdateBaseEnemyBBInfo::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    const AAIController* AIController = OwnerComp.GetAIOwner();
    if (AIController == nullptr) return;

    const auto EnemyOwner = Cast<ABaseSpacecraftEnemy>(AIController->GetPawn());
    if (EnemyOwner == nullptr) return;

    auto BlackboardComponent = OwnerComp.GetBlackboardComponent();
    if (BlackboardComponent == nullptr) return;

    BlackboardComponent->SetValueAsObject(CurrentTargetKey.SelectedKeyName, EnemyOwner->GetCurrentTarget());
}
