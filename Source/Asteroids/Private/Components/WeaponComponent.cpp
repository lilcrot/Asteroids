// A test task by KEFIR
#include "Components/WeaponComponent.h"
#include "Weapons/BaseWeapon.h"

DEFINE_LOG_CATEGORY_STATIC(LogWeaponComponent, All, All);

UWeaponComponent::UWeaponComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UWeaponComponent::BeginPlay()
{
    Super::BeginPlay();
    SpawnWeapons();
}

void UWeaponComponent::SpawnWeapons()
{
    UWorld* World = GetWorld();
    if (World == nullptr) return;

    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = GetOwner();

    for (int i = 0; i < WeaponClasses.Num(); ++i)
    {
        if (!IsValid(WeaponClasses[i]))
        {
            UE_LOG(LogWeaponComponent, Warning, TEXT("The %ith class isn't valid in WeaponClasses"), i);
            continue;
        }
        auto* Weapon = World->SpawnActor<ABaseWeapon>(WeaponClasses[i], SpawnParams);
        if (Weapon == nullptr) continue;

        Weapons.Add(Weapon);
    }
}

void UWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UWeaponComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
    Super::OnComponentDestroyed(bDestroyingHierarchy);

    for (const auto& Weapon : Weapons)
    {
        if (!Weapon) continue;

        Weapon->Destroy();
    }
}

void UWeaponComponent::StartFireByIndex(const int32 Index)
{
    if (!Weapons.IsValidIndex(Index))
    {
        UE_LOG(LogWeaponComponent, Warning, TEXT("StartFireByIndex: invalid index so start fire is failed!"));
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
