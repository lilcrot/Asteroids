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

    auto* AIController = Cast<AAIController>(GetController());
    if (AIController != nullptr)
    {
        AIController->RunBehaviorTree(DefaultBT);
    }
}

void ABaseSpacecraftEnemy::OnDeath()
{
    Super::OnDeath();

    const auto* AIController = Cast<AAIController>(GetController());
    if (AIController != nullptr)
    {
        const auto MyBrainComponent = AIController->GetBrainComponent();
        if (MyBrainComponent != nullptr)
        {
            MyBrainComponent->StopLogic("OnDeath");
        }
    }
}
