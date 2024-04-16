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

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
    TObjectPtr<USkeletalMeshComponent> MeshComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
    TObjectPtr<UBoxComponent> BoxCollision;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
    TObjectPtr<UFloatingPawnMovement> MovementComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
    TObjectPtr<UHealthComponent> HealthComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
    TObjectPtr<UWeaponComponent> WeaponComponent;

protected:
    UFUNCTION()
    void OnActorHitReceive(AActor* SelfActor, AActor* OtherActor, const FVector NormalImpulse, const FHitResult& Hit);

    /* When a spacecraft hit with another actor, how much damage should it do */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage", meta = (MinClamp = "1"))
    int32 HitDamage = 1;

    UFUNCTION(BlueprintNativeEvent)
    void OnDeath_Visual();
    UFUNCTION()
    virtual void OnDeath();

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Health|Death", meta = (Units = "s", ClampMin = "0.1"))
    float LifeSpanOnDeath = 0.1f;
};
