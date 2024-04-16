// A test task by KEFIR

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "Asteroid.generated.h"

class USphereComponent;
class USkeletalMeshComponent;
class UHealthComponent;
class UFloatingPawnMovement;

UCLASS(Abstract, NotBlueprintType, Blueprintable)
class ASTEROIDS_API AAsteroid : public APawn
{
    GENERATED_BODY()

public:
    AAsteroid();

    virtual void Tick(float DeltaTime) override;

    virtual FVector GetVelocity() const override;

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    TObjectPtr<USphereComponent> SphereCollision;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
    TObjectPtr<UHealthComponent> HealthComponent;

    UPROPERTY(BlueprintReadOnly, Category = "Components")
    TObjectPtr<UFloatingPawnMovement> MovementComponent;

    UFUNCTION()
    void OnActorHitReceive(AActor* SelfActor, AActor* OtherActor, const FVector NormalImpulse, const FHitResult& Hit);

    UFUNCTION(BlueprintNativeEvent)
    void OnDeath();

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

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Asteroid|Movement")
    FRotator RotationSpeed = FRotator::ZeroRotator;

    /* When an asteroid hit with another actor, how much damage should it do */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Asteroid|Damage", meta = (MinClamp = "1"))
    int32 HitDamage = 1;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Asteroid|Health|Death", meta = (Units = "s", ClampMin = "0.1"))
    float LifeSpanOnDeath = 0.1f;
};
