// A test task by KEFIR

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "Service_Fire.generated.h"

class UWeaponComponent;

UCLASS()
class ASTEROIDS_API UService_Fire : public UBTService
{
    GENERATED_BODY()
public:
    UService_Fire();

    /* This key object will be set by owner's CurrentTarget */
    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector CurrentTargetKey;

protected:
    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
