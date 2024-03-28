// A test task by KEFIR

#pragma once

#include "CoreMinimal.h"
#include "BaseSpacecraft.h"
#include "InputActionValue.h"

#include "PlayerSpacecraft.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class ASTEROIDS_API APlayerSpacecraft : public ABaseSpacecraft
{
    GENERATED_BODY()

public:
    APlayerSpacecraft();

    virtual void Tick(float DeltaTime) override;

protected:
    virtual void BeginPlay() override;

    UFUNCTION()
    void Move(const FInputActionValue& Value);

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement", meta = (ClampMin = "0.0"))
    float RotationSpeed = 2.5f;

    void FirstWeaponFire();
    void SecondWeaponFire();
    void StopFire();

private:
    void LookToMouse(float DeltaTime);

    void ToggleGamePause();

    virtual void OnDeath() override;

protected:
    //-------------------
    // Enhanced Inputs
    //-------------------

    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent);

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput|Default")
    UInputMappingContext* DefaultContext;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput|Default")
    UInputAction* MovementAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput|Default")
    UInputAction* FirstWeaponFireAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput|Default")
    UInputAction* SecondWeaponFireAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput|Default")
    UInputAction* ToggleGamePauseAction;
};
