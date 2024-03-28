// A test task by KEFIR

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileBase.generated.h"

class USkeletalMeshComponent;
class UBoxComponent;
class UProjectileMovementComponent;

UCLASS()
class ASTEROIDS_API AProjectileBase : public AActor
{
    GENERATED_BODY()

public:
    AProjectileBase();

    virtual void Tick(float DeltaTime) override;

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
    USkeletalMeshComponent* MeshComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
    UBoxComponent* BoxCollision;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
    UProjectileMovementComponent* MovementComponent;

    /* When a projectile collides with another actor, how much damage should it do */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile", meta = (MinClamp = "0"))
    int32 Damage = 1;

    UFUNCTION()
    void OnProjectileOverlapReceive(AActor* OverlappedActor, AActor* OtherActor);

    bool MakePointDamage(AActor* DamageTaker);
};
