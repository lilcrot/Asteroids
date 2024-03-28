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

    UFUNCTION(BlueprintPure, Category = "Enemy")
    AActor* GetCurrentTarget() const;

protected:
    virtual void BeginPlay() override;

    // It can be set by AI perception or etc methods
    UPROPERTY(BlueprintReadWrite, Category = "Enemy")
    AActor* CurrentTarget;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy")
    UBehaviorTree* DefaultBT;

private:
    virtual void OnDeath() override;
};
