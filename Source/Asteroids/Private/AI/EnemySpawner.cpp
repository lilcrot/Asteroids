// A test task by KEFIR
#include "AI/EnemySpawner.h"
#include "Components/ArrowComponent.h"

AEnemySpawner::AEnemySpawner()
{
    PrimaryActorTick.bCanEverTick = true;

    ArrowSpawnDirectionComponent = CreateDefaultSubobject<UArrowComponent>("ArrowSpawnDirectionComponent");
    checkf(ArrowSpawnDirectionComponent, TEXT("ArrowSpawnDirectionComponent doesn't exist!"));
    SetRootComponent(ArrowSpawnDirectionComponent);
}

void AEnemySpawner::BeginPlay()
{
    Super::BeginPlay();
    checkf(RequiredTimeBetweenSpawn > 0.0f, TEXT("RequiredTimeBetweenSpawn must be more than zero!"));
}

void AEnemySpawner::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    SolveQueue();
}

bool AEnemySpawner::TryToSpawnEnemy(const TSubclassOf<AActor> EnemyClass)
{
    UWorld* World = GetWorld();
    if (!EnemyClass || !World) return false;

    if (!CanSpawn())
    {
        AddEnemyToSpawnQueue(EnemyClass);
        return false;
    }

    const FTransform SpawnTransform(GetSpawnDirection(), GetActorLocation(), FVector::OneVector);
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

    const auto Enemy = World->SpawnActor<AActor>(EnemyClass, SpawnTransform, SpawnParams);
    if (!Enemy)
    {
        AddEnemyToSpawnQueue(EnemyClass);
        return false;
    }

    bIsAbilityToSpawnFrozen = true;
    FTimerHandle DelayTimerHandle;
    World->GetTimerManager().SetTimer(
        DelayTimerHandle, [&]() { bIsAbilityToSpawnFrozen = false; }, RequiredTimeBetweenSpawn, false, -1.0f);

    OnEnemySpawned.Broadcast(Enemy);
    return true;
}

void AEnemySpawner::SolveQueue()
{
    if (SpawnQueue.IsEmpty()) return;

    /* without this check it will be infinite loop with duplicate the same value */
    if (CanSpawn())
    {
        if (SpawnQueue.Peek()) TryToSpawnEnemy(*SpawnQueue.Peek());
        SpawnQueue.Pop();
    }
}

bool AEnemySpawner::CanSpawn() const
{
    return !bIsAbilityToSpawnFrozen;
}

FRotator AEnemySpawner::GetSpawnDirection() const
{
    if (!ArrowSpawnDirectionComponent) return GetActorRotation();
    return ArrowSpawnDirectionComponent->GetForwardVector().Rotation();
}

void AEnemySpawner::AddEnemyToSpawnQueue(const TSubclassOf<AActor> EnemyClass)
{
    if (!EnemyClass) return;
    SpawnQueue.Enqueue(EnemyClass);
}
