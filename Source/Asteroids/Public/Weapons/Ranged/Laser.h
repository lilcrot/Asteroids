// A test task by KEFIR

#pragma once

#include "CoreMinimal.h"
#include "Weapons/Ranged/BaseRangedWeapon.h"
#include "Laser.generated.h"

UCLASS(Abstract)
class ASTEROIDS_API ALaser : public ABaseRangedWeapon
{
    GENERATED_BODY()

public:
    ALaser();

    virtual void StartFire() override;
    virtual void StopFire() override;

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Fire", meta = (Units = "cm"))
    float ShotRange = 3000.0f;
};
