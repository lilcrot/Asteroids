// A test task by KEFIR
#include "MyGameInstance.h"
#include "Settings/MyGameUserSettings.h"

void UMyGameInstance::Init()
{
    Super::Init();

    MusicManager = NewObject<UMusicManager>(this, MusicManagerClass);
    checkf(MusicManager, TEXT("MusicManager doesn't exist!"));
    MusicManager->Init(GetWorld());
}

void UMyGameInstance::OnStart()
{
    Super::OnStart();

    if (auto MyGameUserSettings = UMyGameUserSettings::Get())
    {
        MyGameUserSettings->OnGameStarted(GetWorld());
    }
}
