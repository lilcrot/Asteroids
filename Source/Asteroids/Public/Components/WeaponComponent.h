// A test task by KEFIR

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "WeaponComponent.generated.h"

class ABaseWeapon;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ASTEROIDS_API UWeaponComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UWeaponComponent();

    virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;

    /*
        First weapon - 0 | Second weapon - 1 (if any)
        You can compare Index argument with indexes in WeaponClasses
    */
    void StartFireByIndex(const int32 Index);
    void StopFire();

protected:
    virtual void BeginPlay() override;

    /* Classes of weapons that the owner will have */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponComponent")
    TArray<TSubclassOf<ABaseWeapon>> WeaponClasses;

private:
    TObjectPtr<ABaseWeapon> CurrentWeapon;
    TArray<TObjectPtr<ABaseWeapon>> Weapons;
    int32 CurrentWeaponIndex = 0;

    void SpawnWeapons();
};
