// A test task by KEFIR
#pragma once
#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Music/MusicManager.h"

#include "MyGameInstance.generated.h"

UCLASS()
class ASTEROIDS_API UMyGameInstance : public UGameInstance
{
    GENERATED_BODY()
public:
    virtual void Init() override;

    /* This class will be spawned on play */
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
    TSubclassOf<UMusicManager> MusicManagerClass = UMusicManager::StaticClass();

protected:
    virtual void OnStart() override;

private:
    UPROPERTY()
    TObjectPtr<UMusicManager> MusicManager;
};
