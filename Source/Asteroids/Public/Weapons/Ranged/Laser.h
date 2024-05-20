// A test task by KEFIR

#pragma once

#include "CoreMinimal.h"
#include "Weapons/Ranged/BaseRangedWeapon.h"
#include "Laser.generated.h"

class UNiagaraSystem;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnLaserShotsChanged, int32);  // new laser shots value
DECLARE_MULTICAST_DELEGATE_OneParam(FOnReloadingStarted, float);   // reload time in seconds
DECLARE_MULTICAST_DELEGATE(FOnReloadingFinished);

UCLASS(Abstract)
class ASTEROIDS_API ALaser : public ABaseRangedWeapon
{
    GENERATED_BODY()

public:
    ALaser();

    virtual void StartFire() override;
    virtual void StopFire() override;

    FOnLaserShotsChanged OnLaserShotsChanged;

    FOnReloadingStarted OnReloadingStarted;
    FOnReloadingFinished OnReloadingFinished;

    UFUNCTION(BlueprintPure)
    int32 GetCurrentLaserShots() const;

    UFUNCTION(BlueprintPure)
    float GetReloadingTime() const;

protected:
    virtual void BeginPlay() override;

    virtual void MakeShot() override;
    virtual bool CanMakeShot() const override;

    void GetTraceData(FVector& TraceStart, FVector& TraceEnd) const;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Laser", meta = (ClampMin = "100", Units = "cm"))
    float ShotRange = 3000.0f;

    /* Think of the laser as a sphere */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Laser", meta = (ClampMin = "3", Units = "cm"))
    float LaserRadius = 15.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon|Laser", meta = (ClampMin = "1"))
    int32 MaxLaserShots = 10;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon|Laser", meta = (ClampMin = "0.01", Units = "s"))
    float ReloadingTime = 10.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Laser|VFX")
    TObjectPtr<UNiagaraSystem> LaserEffect;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Laser|VFX")
    FName LaserEndVectorParamaterName = "LaserEnd";
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Laser|VFX")
    FName LaserWidthFloatParamaterName = "LaserWidth";

private:
    void PlayLaserEffect(const FHitResult& Hit);

    void SetCurrentLaserShots(const int32 NewLaserShots);
    int32 CurrentLaserShots = 0;

    FTimerHandle ReloadTimerHandle;
    bool bReloading = false;
    void StartReload();
    void StopReload();
};
