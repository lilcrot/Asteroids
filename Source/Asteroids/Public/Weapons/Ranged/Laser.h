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

    virtual void MakeShot() override;
    virtual bool CanMakeShot() const override;

    void GetTraceData(FVector& TraceStart, FVector& TraceEnd) const;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Laser", meta = (Units = "cm"))
    float ShotRange = 3000.0f;

    /* Think of the laser as a sphere */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Laser", meta = (Units = "cm"))
    float LaserRadius = 15.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon|Laser")
    int32 MaxLaserShots = 10;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon|Laser", meta = (ClampMin = "0.01", Units = "s"))
    float ReloadingTime = 10.0f;

private:
    int32 CurrentLaserShots = 0;

    void StartReloadLaserShots();
    bool bReloading = false;
};
