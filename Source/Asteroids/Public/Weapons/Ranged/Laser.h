// A test task by KEFIR

#pragma once

#include "CoreMinimal.h"
#include "Weapons/Ranged/BaseRangedWeapon.h"
#include "Laser.generated.h"

UCLASS()
class ASTEROIDS_API ALaser : public ABaseRangedWeapon
{
    GENERATED_BODY()

public:
    ALaser();

    virtual void StartFire() override;
    virtual void StopFire() override;

protected:
    virtual void BeginPlay() override;
};
