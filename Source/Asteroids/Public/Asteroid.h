// A test task by KEFIR

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "Asteroid.generated.h"

class USkeletalMeshComponent;
class USphereComponent;
class UHealthComponent;
class UFloatingPawnMovement;

UCLASS(Abstract, NotBlueprintType, Blueprintable)
class ASTEROIDS_API AAsteroid : public APawn
{
    GENERATED_BODY()

public:
    AAsteroid();

    virtual void Tick(float DeltaTime) override;

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    USkeletalMeshComponent* MeshComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    USphereComponent* SphereCollision;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
    UHealthComponent* HealthComponent;

    UPROPERTY(BlueprintReadOnly, Category = "Components")
    UFloatingPawnMovement* MovementComponent;

    UFUNCTION()
    void OnActorBeginOverlapReceive(AActor* OverlappedActor, AActor* OtherActor);

    UFUNCTION(BlueprintNativeEvent)
    void OnDeath();

private:
    void StartRecoverVelocity();

    float CurrentVelocityScaleValue = 1.0f;
    FTimerHandle RecoverVelocityTimerHandle;
protected:
    //----------------------
    // Asteroid properties
    //----------------------

    /* The asteroid's acceleration will be randomized when he is spawned according to this range [MaxAcceleration; MinAcceleration] */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Asteroid|Movement", meta = (MinClamp = "0"))
    float MaxAcceleration = 300.0f;

    /* The asteroid's acceleration will be randomized when he is spawned according to this range [MaxAcceleration; MinAcceleration] */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Asteroid|Movement", meta = (MinClamp = "0"))
    float MinAcceleration = 150.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Asteroid|Movement", meta = (MinClamp = "0"))
    float MaxSpeed = 1000.0f;

    /* When an asteroid collides with another actor, how much damage should it do */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Asteroid|Overlap", meta = (MinClamp = "0"))
    float OverlapDamage = 1.0f;

    /* When an asteroid collides with another actor, how much will his speed slow down (1 - nothing change, >1 - slow down) */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Asteroid|Overlap", meta = (MinClamp = "1"))
    float VelocityDivideCoefficient = 1.1f;
};
