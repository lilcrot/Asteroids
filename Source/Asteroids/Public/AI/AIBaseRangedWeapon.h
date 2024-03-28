// A test task by KEFIR

#pragma once

#include "CoreMinimal.h"
#include "Weapons/Ranged/BaseRangedWeapon.h"
#include "AIBaseRangedWeapon.generated.h"

UCLASS(Abstract)
class ASTEROIDS_API AAIBaseRangedWeapon : public ABaseRangedWeapon
{
    GENERATED_BODY()

protected:
    virtual void GetSpawnProjectileData(FTransform& SpawnTransform, FActorSpawnParameters& SpawnParams) override;

    virtual bool CanMakeShot() const override;
};
