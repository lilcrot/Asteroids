// A test task by KEFIR
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "BaseSpacecraft.generated.h"

class USkeletalMeshComponent;
class UBoxComponent;
class UPawnMovementComponent;
class UHealthComponent;

UCLASS()
class ASTEROIDS_API ABaseSpacecraft : public APawn
{
    GENERATED_BODY()

public:
    ABaseSpacecraft();

    virtual void Tick(float DeltaTime) override;

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    USkeletalMeshComponent* MeshComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    UBoxComponent* BoxCollision;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
    UPawnMovementComponent* MovementComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
    UHealthComponent* HealthComponent;
};
