// A test task by KEFIR

#pragma once

#include "CoreMinimal.h"
#include "BaseSpacecraft.h"
#include "InputActionValue.h"

#include "PlayerSpacecraft.generated.h"

class UInputMappingContext;
class UInputAction;
class UNiagaraComponent;

UCLASS()
class ASTEROIDS_API APlayerSpacecraft : public ABaseSpacecraft
{
    GENERATED_BODY()

public:
    APlayerSpacecraft();

    virtual void Tick(float DeltaTime) override;

protected:
    virtual void BeginPlay() override;

private:
    UFUNCTION()
    void Move(const FInputActionValue& Value);
    void OnMoveStarted();
    void OnMoveCompleted();

    void LookToMouse(float DeltaTime);

    void FirstWeaponFire();
    void SecondWeaponFire();
    void StopFire();

    void ToggleGamePause();

    virtual void OnDeath() override;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement", meta = (ClampMin = "0.0"))
    float RotationSpeed = 2.5f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
    TObjectPtr<UNiagaraComponent> EngineFireNiagaraComponent;

protected:
    //-------------------
    // Enhanced Inputs
    //-------------------

    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent);

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput|Default")
    TObjectPtr<UInputMappingContext> DefaultContext;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput|Default")
    TObjectPtr<UInputAction> MovementAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput|Default")
    TObjectPtr<UInputAction> FirstWeaponFireAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput|Default")
    TObjectPtr<UInputAction> SecondWeaponFireAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput|Default")
    TObjectPtr<UInputAction> ToggleGamePauseAction;
};
