// A test task by KEFIR
#include "AI/BTServices/Service_Fire.h"
#include "Components/WeaponComponent.h"
#include "AIController.h"

UService_Fire::UService_Fire() {}

void UService_Fire::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    const AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController) return;

    const APawn* Pawn = AIController->GetPawn();
    if (!Pawn) return;

    auto WeaponComponent = Pawn->FindComponentByClass<UWeaponComponent>();
    if (!WeaponComponent) return;

    WeaponComponent->StartFireByIndex(0);
}
