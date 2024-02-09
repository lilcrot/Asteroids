// A test task by KEFIR

#pragma once

#include "CoreMinimal.h"
#include "Weapons/BaseWeapon.h"
#include "BaseRangedWeapon.generated.h"

UCLASS()
class ASTEROIDS_API ABaseRangedWeapon : public ABaseWeapon
{
    GENERATED_BODY()

public:
    ABaseRangedWeapon();

    virtual void StartFire() override;
    virtual void StopFire() override;

protected:
    virtual void BeginPlay() override;

};
