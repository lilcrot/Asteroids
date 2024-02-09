// A test task by KEFIR
#include "Components/WeaponComponent.h"
#include "Weapons/BaseWeapon.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseWeapon, All, All);

UWeaponComponent::UWeaponComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UWeaponComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UWeaponComponent::OnComponentDestroyed(bool bDestroyingHierarchy) 
{
    Super::OnComponentDestroyed(bDestroyingHierarchy);

    for (auto* Weapon : Weapons)
    {
        if (Weapon) Weapon->Destroy();
    }
}

void UWeaponComponent::StartFireByIndex(const int32 Index)
{
    if (!Weapons.IsValidIndex(Index))
    {
        UE_LOG(LogBaseWeapon, Warning, TEXT("StartFireByIndex: invalid index so start fire is failed!"));
        return;
    }
    if (Weapons[Index])
    {
        CurrentWeapon = Weapons[Index];
        CurrentWeapon->StartFire();
    }
}

void UWeaponComponent::StopFire()
{
    if (!CurrentWeapon) return;
    CurrentWeapon->StopFire();
}
