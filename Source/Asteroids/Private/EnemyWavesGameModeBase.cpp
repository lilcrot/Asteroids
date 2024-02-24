// A test task by KEFIR
#include "EnemyWavesGameModeBase.h"
#include "AI/EnemySpawner.h"
#include "Kismet/GameplayStatics.h"
#include "Player/PlayerSpacecraft.h"

DEFINE_LOG_CATEGORY_STATIC(LogEnemyWavesGMBase, All, All);

AEnemyWavesGameModeBase::AEnemyWavesGameModeBase()
{
    DefaultPawnClass = APlayerSpacecraft::StaticClass();
}

void AEnemyWavesGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
    Super::InitGame(MapName, Options, ErrorMessage);

    {
        checkf(EnemyWavePeriodTime > 0.0f, TEXT("EnemyWavePeriodTime must be more than zero!"));
        checkf(WavePeriodTimeAdditionalModifier >= 0.0f, TEXT("WavePeriodTimeAdditionalModifier must be more or equal than zero!"));

        checkf(EnemyStrengthMultiplier >= 1.0f, TEXT("EnemyStrengthMultiplier must be more or equal than one!"));
        checkf(PointsAdditionalModifier >= 0.0f, TEXT("PointsAdditionalModifier must be more or equal than zero!"));
    }

    CurrentWavePoints = InitialPoints;
}

void AEnemyWavesGameModeBase::BeginPlay()
{
    Super::BeginPlay();

    if (const auto PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
    {
        PlayerController->SetShowMouseCursor(true);
    }

    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemySpawner::StaticClass(), EnemySpawners);
    if (EnemySpawners.IsEmpty())
    {
        UE_LOG(LogEnemyWavesGMBase, Error, TEXT("Enemy spawners are not found on the map, so the enemy wave will always be unsuccessful!"));
        return;
    }
    for (const auto It : EnemySpawners)
    {
        const auto Spawner = Cast<AEnemySpawner>(It);
        if (!Spawner) continue;
        Spawner->OnEnemySpawned.AddDynamic(this, &ThisClass::OnEnemySpawnedByWave);
    }

    StartNewWave(false);  // we don't need progression on the first wave
}

void AEnemyWavesGameModeBase::StartNewWave(const bool bApplyProgression)
{
    CurrentWaveNumber++;
    if (bApplyProgression) ApplyProgression();

    UE_LOG(LogEnemyWavesGMBase, Display, TEXT("The new %i-th wave with %f points !"), CurrentWaveNumber, CurrentWavePoints);


    const TArray<TSubclassOf<AActor>> EnemyPool = BuildEnemyPool(CurrentWavePoints);
    RemainingEnemies += EnemyPool.Num();
    for (const auto EnemyClass : EnemyPool)
    {
        const int32 RandomIndex = FMath::RandRange(0, EnemySpawners.Num() - 1);
        if (const auto Spawner = Cast<AEnemySpawner>(EnemySpawners[RandomIndex]))
        {
            Spawner->TryToSpawnEnemy(EnemyClass);
        }
    }

    if (UWorld* World = GetWorld())
    {
        FTimerDelegate TimerDelegate;
        TimerDelegate.BindUObject(this, &ThisClass::StartNewWave, true);

        World->GetTimerManager().SetTimer(NewWaveByPeriodTimerHandle, TimerDelegate, EnemyWavePeriodTime, false, -1.0f);
    }
}

void AEnemyWavesGameModeBase::ApplyProgression()
{
    CurrentWavePoints = (InitialPoints * (CurrentWaveNumber * EnemyStrengthMultiplier) + PointsAdditionalModifier);

    /* Every 10 waves EnemyWavePeriodTime modifier is added to the current */
    if (CurrentWaveNumber % 10 == 0)
    {
        EnemyWavePeriodTime += WavePeriodTimeAdditionalModifier;
    }
}

void AEnemyWavesGameModeBase::OnEnemySpawnedByWave(AActor* Enemy)
{
    if (!Enemy) return;

    Enemy->OnDestroyed.AddDynamic(this, &ThisClass::OnEnemyFromWaveDestroyed);
}

void AEnemyWavesGameModeBase::OnEnemyFromWaveDestroyed(AActor* DestroyedEnemy)
{
    if (!DestroyedEnemy) return;

    DestroyedEnemy->OnDestroyed.RemoveAll(this);

    if (--RemainingEnemies <= 0)
    {
        if (UWorld* World = GetWorld())
        {
            World->GetTimerManager().ClearTimer(NewWaveByPeriodTimerHandle);
            StartNewWave();
        }
    }
}

TArray<TSubclassOf<AActor>> AEnemyWavesGameModeBase::BuildEnemyPool(float WavePoints)
{
    /*  <EnemyClass, NumberToSpawn> */
    TMap<TSubclassOf<AActor>, int32> OutEnemies;

    /* The higest cost is on top | The cheapest cost is on tail */
    EnemyClassesInfoMap.ValueSort(
        [&](const FEnemyClassInWaveInfo& A, const FEnemyClassInWaveInfo& B) { return A.PointCost > B.PointCost; });

    TPair<TSubclassOf<AActor>, FEnemyClassInWaveInfo> TheCheapestPair(nullptr, FEnemyClassInWaveInfo{FLT_MAX, 0});
    for (const auto Pair : EnemyClassesInfoMap)
    {
        if (Pair.Value.PointCost < TheCheapestPair.Value.PointCost) TheCheapestPair = Pair;
    }

    const auto CanContinue = [&]() -> bool
    {
        const bool CurrentWavePointsCondition = WavePoints != 0.0f && WavePoints > TheCheapestPair.Value.PointCost;
        if (!OutEnemies.Contains(TheCheapestPair.Key)) return CurrentWavePointsCondition;

        return CurrentWavePointsCondition && OutEnemies[TheCheapestPair.Key] != TheCheapestPair.Value.MaxSpawnPerWave;
    };

    const auto HowMuchCanBuy = [&](const float EnemyCost, const int32 EnemyMaxCountOnMap) -> int32
    {
        if (WavePoints < EnemyCost) return 0;
        checkf(EnemyMaxCountOnMap > 0, TEXT("MaxCountOnMap of the enemy in Wave must be more than zero!"));
        checkf(EnemyCost > 0.0f, TEXT("EnemyCost in Wave must be more than zero!"));

        return FMath::Clamp(WavePoints / EnemyCost, 0, EnemyMaxCountOnMap);
    };

    // Which part algorithm will buy from the maximum possible for one itteration of EnemyClass.
    // We don't want buy one class for all the points
    constexpr int32 DivisorOfMaxPossible = 2;

    while (CanContinue())
    {
        for (const auto& Pair : EnemyClassesInfoMap)
        {
            const auto EnemyCost = Pair.Value.PointCost;
            const auto EnemyMaxCountOnMap = Pair.Value.MaxSpawnPerWave;

            const int32 NumberCanBuy = HowMuchCanBuy(EnemyCost, EnemyMaxCountOnMap);
            if (NumberCanBuy == 0) continue;

            const int32 AlreadyHave = OutEnemies.FindOrAdd(Pair.Key);
            const int32 NeedToBuy = FMath::Clamp(NumberCanBuy / DivisorOfMaxPossible, 1, EnemyMaxCountOnMap - AlreadyHave);

            WavePoints -= NeedToBuy * EnemyCost;
            OutEnemies[Pair.Key] += NeedToBuy;
        }
    }

    TArray<TSubclassOf<AActor>> Result;
    for (const auto& Pair : OutEnemies)
    {
        for (int32 i = 0; i < Pair.Value; i++)
            Result.Add(Pair.Key);
    }

    return Result;
}