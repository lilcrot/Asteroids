// A test task by KEFIR

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"

#include "BTT_ChasingTarget.generated.h"

UCLASS()
class ASTEROIDS_API UBTT_ChasingTarget : public UBTTaskNode
{
    GENERATED_BODY()
public:
    UBTT_ChasingTarget();

    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector TargetKey;

protected:
    virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;

private:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

    virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds);

    void FinishLatentTaskWithError(UBehaviorTreeComponent& OwnerComp, EBTNodeResult::Type TaskResult, const FString ErrorMessage);
};