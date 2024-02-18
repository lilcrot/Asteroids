// A test task by KEFIR
#include "AI/BTTasks/BTT_ChasingTarget.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogTask_ChasingPlayer, All, All);

UBTT_ChasingTarget::UBTT_ChasingTarget()
{
    bNotifyTick = true;
}

void UBTT_ChasingTarget::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

    using Result = EBTNodeResult::Type;

    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController)
    {
        FinishLatentTaskWithError(OwnerComp, Result::Failed, "UBTT_ChasingTarget doesn't has a AIController.");
        return;
    }

    auto BlackboardComponent = OwnerComp.GetBlackboardComponent();
    if (!BlackboardComponent)
    {
        FinishLatentTaskWithError(OwnerComp, Result::Failed, "UBTT_ChasingTarget doesn't has a BlackboardComponent.");
        return;
    }

    const auto CurrentTarget = Cast<AActor>(BlackboardComponent->GetValueAsObject(TargetKey.SelectedKeyName));
    const auto NavResult = AIController->MoveToActor(CurrentTarget);
    if (NavResult == EPathFollowingRequestResult::Type::Failed)
    {
        FinishLatentTaskWithError(OwnerComp, Result::Failed, "MoveRequestToTarget is failed so BTT_ChasingTarget if failed.");
    }
    else if (NavResult == EPathFollowingRequestResult::Type::AlreadyAtGoal)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Type::Succeeded);
    }
}

EBTNodeResult::Type UBTT_ChasingTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    return EBTNodeResult::Type::InProgress;
}

void UBTT_ChasingTarget::FinishLatentTaskWithError(UBehaviorTreeComponent& OwnerComp, EBTNodeResult::Type TaskResult,  //
    const FString ErrorMessage)                                                                                        //
{
    UE_LOG(LogTask_ChasingPlayer, Error, TEXT("%s"), *ErrorMessage);
    FinishLatentTask(OwnerComp, TaskResult);
}
