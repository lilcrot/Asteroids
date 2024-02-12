// A test task by KEFIR
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "BaseSpacecraft.generated.h"

class USkeletalMeshComponent;
class UBoxComponent;
class UFloatingPawnMovement;
class UHealthComponent;
class UWeaponComponent;

UCLASS()
class ASTEROIDS_API ABaseSpacecraft : public APawn
{
    GENERATED_BODY()

public:
    ABaseSpacecraft();

    virtual void Tick(float DeltaTime) override;

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
    USkeletalMeshComponent* MeshComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
    UBoxComponent* BoxCollision;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
    UFloatingPawnMovement* MovementComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
    UHealthComponent* HealthComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
    UWeaponComponent* WeaponComponent;

    /* When a spacecraft collides with another actor, how much damage should it do */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Overlap", meta = (MinClamp = "0"))
    int32 OverlapDamage = 1;

protected:
    UFUNCTION()
    virtual void OnActorBeginOverlapReceive(AActor* OverlappedActor, AActor* OtherActor);
};
