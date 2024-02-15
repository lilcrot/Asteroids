// A test task by KEFIR

#pragma once

#include "CoreMinimal.h"
#include "BaseSpacecraft.h"

#include "BaseSpacecraftEnemy.generated.h"

class UBehaviorTree;

UCLASS()
class ASTEROIDS_API ABaseSpacecraftEnemy : public ABaseSpacecraft
{
    GENERATED_BODY()

public:
    ABaseSpacecraftEnemy();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy")
    UBehaviorTree* DefaultBT = nullptr;
};
