// A test task by KEFIR
#pragma once
#include "CoreMinimal.h"

#include "MusicManager.generated.h"

class AMusicSystem;

USTRUCT(Blueprintable)
struct FMusicSystemInfo
{
    GENERATED_BODY()

    /* The world in which the object with selected class (MysicSystemClass) will exist. */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSoftObjectPtr<UWorld> World;

    /* A system with this class will exist in the selected world and will control all music. */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSubclassOf<AMusicSystem> MysicSystemClass;
};

UCLASS(Blueprintable, BlueprintType)
class ASTEROIDS_API UMusicManager : public UObject
{
    GENERATED_BODY()
    friend class UMyGameInstance;

private:
    UMusicManager();
    void Init(UWorld* World);

    void OnPostWorldInitialization(UWorld* World, const UWorld::InitializationValues IVS);

protected:
    UFUNCTION(BlueprintNativeEvent)
    void OnInitialized(UWorld* World);

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MusicManager")
    TArray<FMusicSystemInfo> MusicSystems;

private:
    void SpawnMusicSystemByWorld(UWorld* World);
};
