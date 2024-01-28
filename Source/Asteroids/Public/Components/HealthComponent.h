// A test task by KEFIR

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeathSignature);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ASTEROIDS_API UHealthComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UHealthComponent();

    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    /* safe set health with clamp between zero and max health */
    UFUNCTION(BlueprintCallable, Category = "Health")
    void SetHealth(const int32 NewHealth);

    UFUNCTION(BlueprintPure, Category = "Health")
    int32 GetHealth() const { return CurrentHealth; }

    /* return health limit */
    UFUNCTION(BlueprintPure, Category = "Health")
    int32 GetMaxHealth() const { return MaxHealth; }

    UFUNCTION(BlueprintPure, Category = "Health")
    bool IsDead() const { return CurrentHealth <= 0; }

protected:
    virtual void BeginPlay() override;

    UFUNCTION()
    void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType,  //
        class AController* InstigatedBy, AActor* DamageCauser);                                    //

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health")
    int32 MaxHealth = 4;

private:
    int CurrentHealth = MaxHealth;

public:
    //----------------------
    // Health Delegates
    //----------------------

    UPROPERTY(BlueprintAssignable)
    FOnDeathSignature OnDeath;
};