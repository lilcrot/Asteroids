// A test task by KEFIR
#include "AI/BTServices/Service_Fire.h"
#include "Components/WeaponComponent.h"
#include "AIController.h"

DEFINE_LOG_CATEGORY_STATIC(LogAIServiceFire, All, All);

UService_Fire::UService_Fire()
{
    Interval = 1.0f;
}

void UService_Fire::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    auto WeaponComponent = GetOwnerWeaponComponent(OwnerComp);
    if (WeaponComponent == nullptr)
    {
        UE_LOG(LogAIServiceFire, Error, TEXT("Pawn owner doesn't has WeaponComponent, but it's implied"));
        return;
    }

    WeaponComponent->StartFireByIndex(0);
}

UWeaponComponent* UService_Fire::GetOwnerWeaponComponent(const UBehaviorTreeComponent& OwnerComp) const
{
    const AAIController* AIController = OwnerComp.GetAIOwner();
    if (AIController == nullptr) return nullptr;

    const auto PawnOwner = AIController->GetPawn();
    if (PawnOwner == nullptr) return nullptr;

    return PawnOwner->FindComponentByClass<UWeaponComponent>();
}
