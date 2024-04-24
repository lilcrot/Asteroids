// A test task by KEFIR
#pragma once

#include "CoreMinimal.h"
#include "Components/WeaponComponent.h"

#include "TestWeaponComponent.generated.h"

class ABaseWeapon;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent), Blueprintable)
class ASTEROIDS_API UTestWeaponComponent : public UWeaponComponent
{
    GENERATED_BODY()

public:
    UTestWeaponComponent();

    TArray<TSubclassOf<ABaseWeapon>> GetWeaponClasses() const;
};