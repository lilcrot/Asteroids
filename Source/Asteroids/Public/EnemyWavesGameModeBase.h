// A test task by KEFIR

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "EnemyWavesGameModeBase.generated.h"

USTRUCT(Blueprintable)
struct FEnemyClassInWaveInfo
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float PointCost = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxSpawnPerWave = 10;
};

UCLASS(Abstract)
class ASTEROIDS_API AEnemyWavesGameModeBase : public AGameModeBase
{
    GENERATED_BODY()

public:
    AEnemyWavesGameModeBase();

    virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnemyWaves")
    TMap<TSubclassOf<AActor>, FEnemyClassInWaveInfo> EnemyClassesInfoMap;

    /* Every time this time expires, a new wave will be spawned */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnemyWaves", meta = (ClamMin = "0.01", Units = "s"))
    float EnemyWavePeriodTime = 10.0f;

    /* The first wave will have this amount of points so it's the starting point for the progression */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnemyWaves|Progression")
    float InitialPoints = 10.0f;

    /* NewWavePoints = InitialPoints * ( CurrentWaveNumber * EnemyStrengthMultiplier ) + PointsAdditionalModifier */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnemyWaves|Progression", meta = (ClamMin = "0.01"))
    float EnemyStrengthMultiplier = 1.1f;

    /* NewWavePoints = InitialPoints * ( CurrentWaveNumber * EnemyStrengthMultiplier ) + PointsAdditionalModifier */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnemyWaves|Progression", meta = (ClamMin = "0"))
    float PointsAdditionalModifier = 5.0f;

    /* Every 10 waves, this modifier will be added to the current EnemyWavePeriodTime */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnemyWaves|Progression", meta = (ClamMin = "0", Units = "s"))
    float WavePeriodTimeAdditionalModifier = 3.0f;

private:
    void StartNewWave(const bool bApplyProgression = true);
    void ApplyProgression();

    TArray<TSubclassOf<AActor>> BuildEnemyPool(float WavePoints);

private:
    TArray<AActor*> EnemySpawners;

    float CurrentWavePoints = 0.0f;
    int32 CurrentWaveNumber = 0;

    FTimerHandle NewWaveByPeriodTimerHandle;

    UFUNCTION()
    void OnEnemySpawnedByWave(AActor* Enemy);

    UFUNCTION()
    void OnEnemyFromWaveDestroyed(AActor* DestroyedEnemy);
    int32 RemainingEnemies = 0;
};