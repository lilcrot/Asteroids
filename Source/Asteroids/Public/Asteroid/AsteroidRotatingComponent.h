// A test task by KEFIR
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/MovementComponent.h"

#include "AsteroidRotatingComponent.generated.h"

UCLASS(ClassGroup = Movement, meta = (BlueprintSpawnableComponent), HideCategories = (Velocity))
class ASTEROIDS_API UAsteroidRotatingComponent : public UMovementComponent
{
    GENERATED_BODY()
public:
    virtual void InitializeComponent() override;
    virtual void OnRegister() override;

    virtual void BeginPlay();

    virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AsteroidRotatingComponent",
        meta = (MinClamp = "0", MaxClamp = "360", Units = "Deg"))
    float MinRandomPitch = 0.0f;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AsteroidRotatingComponent",
        meta = (MinClamp = "0.0", MaxClamp = "360", Units = "Deg"))
    float MaxRandomPitch = 0.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AsteroidRotatingComponent",
        meta = (MinClamp = "0", MaxClamp = "360", Units = "Deg"))
    float MinRandomYaw = 0.0f;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AsteroidRotatingComponent",
        meta = (MinClamp = "0", MaxClamp = "360", Units = "Deg"))
    float MaxRandomYaw = 0.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AsteroidRotatingComponent",
        meta = (MinClamp = "0", MaxClamp = "360", Units = "Deg"))
    float MinRandomRoll = 0.0f;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AsteroidRotatingComponent",
        meta = (MinClamp = "0", MaxClamp = "360", Units = "Deg"))
    float MaxRandomRoll = 0.0f;

private:
    FRotator RotationRate = FRotator::ZeroRotator;
};
