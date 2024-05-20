// A test task by KEFIR
#include "Music/MusicManager.h"
#include "Music/MusicSystem.h"

DEFINE_LOG_CATEGORY_STATIC(LogMusicManager, All, All);

UMusicManager::UMusicManager() {}

void UMusicManager::Init(UWorld* World)
{
    SpawnMusicSystemByWorld(World);
    FWorldDelegates::OnPostWorldInitialization.AddUObject(this, &ThisClass::OnPostWorldInitialization);

    OnInitialized(World);
}
void UMusicManager::OnInitialized_Implementation(UWorld* World) {}

void UMusicManager::OnPostWorldInitialization(UWorld* World, const UWorld::InitializationValues IVS)
{
    SpawnMusicSystemByWorld(World);
}

void UMusicManager::SpawnMusicSystemByWorld(UWorld* World)
{
    if (!World)
    {
        UE_LOG(LogMusicManager, Error, TEXT("SpawnMusicSystemByWorld is failed because World ins't valid"));
        return;
    }

    const FString MapName = World->GetMapName();
    const FMusicSystemInfo* FoundInfo = MusicSystems.FindByPredicate(
        [MapName](const FMusicSystemInfo& Info)
        {
            if (!Info.World) return false;
            return MapName.Contains(Info.World->GetMapName());
        });

    if (!FoundInfo)
    {
        UE_LOG(LogMusicManager, Display, TEXT("MusicSystemsMap doesn't contain new world(%s)"), *World->GetMapName());
        return;
    }

    const auto NewMusicSystem = World->SpawnActor<AMusicSystem>(FoundInfo->MysicSystemClass);
    checkf(NewMusicSystem, TEXT("NewMusicSystem isn't valid"));
}
