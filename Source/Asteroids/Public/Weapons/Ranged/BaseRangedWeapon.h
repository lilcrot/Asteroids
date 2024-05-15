// A test task by KEFIR

#pragma once

#include "CoreMinimal.h"
#include "Weapons/BaseWeapon.h"
#include "BaseRangedWeapon.generated.h"

/* This weapon has no skel mesh, muzzle, etc. - everything is on the spacecraft */

class AProjectileBase;

UCLASS(HideDropdown)
class ASTEROIDS_API ABaseRangedWeapon : public ABaseWeapon
{
    GENERATED_BODY()

public:
    ABaseRangedWeapon();

    virtual void StartFire() override;
    virtual void StopFire() override;

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    FName OwnerMuzzleSocketName = "Socket_WeaponMuzzle";

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    TSubclassOf<AProjectileBase> ProjectileClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Fire", meta = (ClampMin = "0.001", Units = "s"))
    float ShootRate = 0.2f;

    virtual void GetSpawnProjectileData(FTransform& SpawnTransform, FActorSpawnParameters& SpawnParams);
    virtual FTransform GetMuzzleSocketTransform() const;

    virtual bool CanMakeShot() const { return true; }

    virtual void MakeShot();
    FTimerHandle FireHandle;

    /*  Note: Automatically called in the MakeShot function of this class,
        if the logic of the function is not inherited, then you must call it yourself */
    UFUNCTION(BlueprintNativeEvent)
    void PlayShotEffect();
};
