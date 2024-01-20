// A test task by KEFIR
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BaseSpacecraft.generated.h"

class USkeletalMeshComponent;
class UBoxComponent;

UCLASS()
class ASTEROIDS_API ABaseSpacecraft : public APawn
{
    GENERATED_BODY()

public:
    ABaseSpacecraft();

    virtual void Tick(float DeltaTime) override;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    USkeletalMeshComponent* MeshComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    UBoxComponent* BoxCollision;

    virtual void BeginPlay() override;
};
