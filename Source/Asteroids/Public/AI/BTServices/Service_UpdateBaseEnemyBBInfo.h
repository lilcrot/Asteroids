// A test task by KEFIR

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "Service_UpdateBaseEnemyBBInfo.generated.h"

UCLASS()
class ASTEROIDS_API UService_UpdateBaseEnemyBBInfo : public UBTService
{
    GENERATED_BODY()
public:
    UService_UpdateBaseEnemyBBInfo();

    /* This key object will be set by owner's CurrentTarget */
    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector CurrentTargetKey;

protected:
    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};