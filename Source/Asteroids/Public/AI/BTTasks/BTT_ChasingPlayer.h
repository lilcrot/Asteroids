// A test task by KEFIR

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"

#include "BTT_ChasingPlayer.generated.h"

UCLASS()
class ASTEROIDS_API UBTT_ChasingPlayer : public UBTTaskNode
{
    GENERATED_BODY()
public:
    UBTT_ChasingPlayer();

private:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
