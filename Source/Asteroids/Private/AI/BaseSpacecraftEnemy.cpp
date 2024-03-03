// A test task by KEFIR
#include "AI/BaseSpacecraftEnemy.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"

ABaseSpacecraftEnemy::ABaseSpacecraftEnemy()
{
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

AActor* ABaseSpacecraftEnemy::GetCurrentTarget() const
{
    return CurrentTarget;
}

void ABaseSpacecraftEnemy::BeginPlay()
{
    Super::BeginPlay();

    if (auto* AIController = Cast<AAIController>(GetController()))
    {
        AIController->RunBehaviorTree(DefaultBT);
    }
}
