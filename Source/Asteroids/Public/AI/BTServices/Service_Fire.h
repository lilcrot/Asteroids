// A test task by KEFIR

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "Service_Fire.generated.h"

UCLASS()
class ASTEROIDS_API UService_Fire : public UBTService
{
    GENERATED_BODY()
public:
    UService_Fire();

protected:
    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
