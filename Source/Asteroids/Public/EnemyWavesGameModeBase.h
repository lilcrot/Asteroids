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

USTRUCT(Blueprintable)
struct FCurrentWaveInfo
{
    GENERATED_USTRUCT_BODY()

    int32 WaveNumber = 0;
    int32 RemainingEnemies = 0;

    float EnemyWavePeriodTime = 0.0f;
    float WavePoints = 0.0f;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGamePauseChangedDelegate, bool, bIsPaused);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNewWaveHasStarted, FCurrentWaveInfo, WaveInfo);

UCLASS(Abstract)
class ASTEROIDS_API AEnemyWavesGameModeBase : public AGameModeBase
{
    GENERATED_BODY()

public:
    AEnemyWavesGameModeBase();

    virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

    UPROPERTY(BlueprintAssignable, Category = "GameMode")
    FOnGamePauseChangedDelegate OnGamePauseChangedEvent;

    UPROPERTY(BlueprintAssignable, Category = "GameMode")
    FOnNewWaveHasStarted OnNewWaveHasStarted;

    UFUNCTION(BlueprintPure)
    FCurrentWaveInfo GetCurrentWaveInfo() const;

protected:
    virtual void BeginPlay() override;

    virtual bool SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate = FCanUnpause()) override;
    virtual bool ClearPause() override;

protected:
    //-----------------------
    //      Enemy Waves
    //-----------------------

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnemyWaves")
    TMap<TSubclassOf<AActor>, FEnemyClassInWaveInfo> EnemyClassesInfoMap;

    /* Every time this time expires, a new wave will be spawned */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnemyWaves", meta = (ClamMin = "0.01", Units = "s"))
    float InitEnemyWavePeriodTime = 10.0f;

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
    UPROPERTY()
    TArray<TObjectPtr<AActor>> EnemySpawners;

    FCurrentWaveInfo CurrentWaveInfo;

    FTimerHandle NewWaveByPeriodTimerHandle;

    UFUNCTION()
    void OnEnemySpawnedByWave(AActor* Enemy);

    UFUNCTION()
    void OnEnemyFromWaveDestroyed(AActor* DestroyedEnemy);
};
