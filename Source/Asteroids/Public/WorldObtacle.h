// A test task by KEFIR

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WorldObtacle.generated.h"

class UBoxComponent;

UCLASS()
class ASTEROIDS_API AWorldObtacle : public AActor
{
    GENERATED_BODY()

public:
    AWorldObtacle();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    UBoxComponent* DeathCollision;

    UFUNCTION()
    void OnDeathCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent,       //
        AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);  //
};
