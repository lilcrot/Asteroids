// A test task by KEFIR
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

class UArrowComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemySpawned, AActor*, Enemy);

UCLASS()
class ASTEROIDS_API AEnemySpawner : public AActor
{
    GENERATED_BODY()

public:
    AEnemySpawner();

    virtual void Tick(float DeltaTime) override;

    bool TryToSpawnEnemy(const TSubclassOf<AActor> EnemyClass);

    UPROPERTY(BlueprintAssignable)
    FOnEnemySpawned OnEnemySpawned;

protected:
    virtual void BeginPlay() override;

    /* After a some enemy is spawned then spawn ability will be frozen for this time */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnemySpawner", meta = (ClamMin = "0", Units = "s"))
    float RequiredTimeBetweenSpawn = 0.7f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
    UArrowComponent* ArrowSpawnDirectionComponent;

    void AddEnemyToSpawnQueue(const TSubclassOf<AActor> EnemyClass);

private:
    TQueue<TSubclassOf<AActor>> SpawnQueue;
    void SolveQueue();

    bool CanSpawn() const;

    FRotator GetSpawnDirection() const;

    bool bIsAbilityToSpawnFrozen = false;
};
