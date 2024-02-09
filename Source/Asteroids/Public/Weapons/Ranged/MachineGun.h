// A test task by KEFIR

#pragma once

#include "CoreMinimal.h"
#include "Weapons/Ranged/BaseRangedWeapon.h"
#include "MachineGun.generated.h"

UCLASS()
class ASTEROIDS_API AMachineGun : public ABaseRangedWeapon
{
    GENERATED_BODY()

public:
    AMachineGun();

    virtual void StartFire() override;
    virtual void StopFire() override;

protected:
    virtual void BeginPlay() override;
};
