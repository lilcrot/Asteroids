// A test task by KEFIR

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"

UCLASS()
class ASTEROIDS_API ABaseWeapon : public AActor
{
    GENERATED_BODY()

public:
    ABaseWeapon();

    virtual void Tick(float DeltaTime) override;

    virtual void StartFire();
    virtual void StopFire();

    virtual bool TryAttachToOwner();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    FName OwnerAttackSocketName = "";

private:
    bool bFiring = false;
};
