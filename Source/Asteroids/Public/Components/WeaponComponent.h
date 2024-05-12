// A test task by KEFIR

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "WeaponComponent.generated.h"

class ABaseWeapon;

DECLARE_MULTICAST_DELEGATE(FOnAllWeaponsSpawned);

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

    FOnAllWeaponsSpawned OnAllWeaponsSpawned;

    template <typename T>
    T* GetWeaponByClass() const;

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

template <typename T>
inline T* UWeaponComponent::GetWeaponByClass() const
{
    const int32 Index = Weapons.IndexOfByPredicate([&](const auto& Weapon) { return Weapon ? Weapon->IsA(T::StaticClass()) : false; });

    return Weapons.IsValidIndex(Index) ? Cast<T>(Weapons[Index]) : nullptr;
}
