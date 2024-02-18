// A test task by KEFIR
#include "AI/BTServices/Service_Fire.h"
#include "Components/WeaponComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/BaseSpacecraftEnemy.h"

DEFINE_LOG_CATEGORY_STATIC(LogAIServiceFire, All, All);

UService_Fire::UService_Fire()
{
    Interval = 0.5f;
}

void UService_Fire::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    const AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController) return;

    const auto EnemyOwner = Cast<ABaseSpacecraftEnemy>(AIController->GetPawn());
    if (!EnemyOwner) return;

    auto BlackboardComponent = OwnerComp.GetBlackboardComponent();
    if (!BlackboardComponent) return;

    BlackboardComponent->SetValueAsObject(CurrentTargetKey.SelectedKeyName, EnemyOwner->GetCurrentTarget());

    auto WeaponComponent = EnemyOwner->FindComponentByClass<UWeaponComponent>();
    if (!WeaponComponent)
    {
        UE_LOG(LogAIServiceFire, Error, TEXT("Owner doesn't have WeaponComponent, but it's implied"));
        return;
    }

    WeaponComponent->StartFireByIndex(0);
}

