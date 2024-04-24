#include "Tests/Components/TestWeaponComponent.h"

UTestWeaponComponent::UTestWeaponComponent() {}

TArray<TSubclassOf<ABaseWeapon>> UTestWeaponComponent::GetWeaponClasses() const
{
    return WeaponClasses;
}