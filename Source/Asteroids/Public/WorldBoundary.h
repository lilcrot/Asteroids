// A test task by KEFIR

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WorldBoundary.generated.h"

class UBoxComponent;

UCLASS()
class ASTEROIDS_API AWorldBoundary : public AActor
{
    GENERATED_BODY()

public:
    AWorldBoundary();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    UBoxComponent* DeathCollision;

    UFUNCTION()
    void OnDeathCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent,       //
        AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);  //
};
