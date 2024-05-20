// A test task by KEFIR
#include "Music/MusicSystem.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogMusicSystem, All, All);

AMusicSystem::AMusicSystem() {}

void AMusicSystem::BeginPlay()
{
    Super::BeginPlay();
    checkf(DefaultMusicSound, TEXT("DefaultMusicSound isn't set for %s"), *GetClass()->GetName());

    UWorld* World = GetWorld();

#if UE_BUILD_DEVELOPMENT
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(World, AMusicSystem::StaticClass(), FoundActors);
    checkf(FoundActors.Num() == 1, TEXT("Only one instance of the Music System can exist per level!"));
#endif  // UE_BUILD_DEVELOPMENT

    PlayDefaultMusic(FMusicFadeInformation(), 0.0f);
}

void AMusicSystem::PlayDefaultMusic(const FMusicFadeInformation FadeOut, const float StartTime)
{
    PlayNewMusic(DefaultMusicSound, FadeOut, DefaultMusicSoundFadeInInfo, StartTime);
}

void AMusicSystem::PlayNewMusic(USoundBase* NewMusic, const FMusicFadeInformation FadeOut,  //
    const FMusicFadeInformation FadeIn, const float StartTime)                              //
{
    UWorld* World = GetWorld();
    if (!World) return;

    if (!NewMusic)
    {
        UE_LOG(LogMusicSystem, Warning, TEXT("PlayNewMusic is failed because NewMusic sounds ins't valid"));
        return;
    }

    UAudioComponent* OldMusic = CurrentMusicAudioComponent;
    if (OldMusic && FadeOut.Duration > 0.0f && !IsPaused())  // on pause timer doesn't work
    {
        OldMusic->FadeOut(FadeOut.Duration, FadeOut.VolumeLevel, FadeOut.Curve);

        FTimerDelegate NewMusicFadeInTimerDelegate;
        NewMusicFadeInTimerDelegate.BindUFunction(this, FName("PlayMusicWithFadeIn"), NewMusic, FadeIn, StartTime);

        World->GetTimerManager().SetTimer(OldMusicFadeOutDelayTimerHandle, NewMusicFadeInTimerDelegate, FadeOut.Duration, false, -1.0f);
    }
    else
    {
        PlayMusicWithFadeIn(NewMusic, FadeIn, StartTime);
    }
}
void AMusicSystem::PlayMusicWithFadeIn(USoundBase* NewMusic, const FMusicFadeInformation FadeIn, const float StartTime)
{
    if (CurrentMusicAudioComponent)
    {
        CurrentMusicAudioComponent->Stop();  // auto-destroy of old music audio component
    }

    CurrentMusicAudioComponent = UGameplayStatics::CreateSound2D(GetWorld(), NewMusic);
    if (!CurrentMusicAudioComponent)
    {
        UE_LOG(LogMusicSystem, Warning, TEXT("PlayMusicWithFadeIn is failed because new AudioComponent could not be created"));
        return;
    }

    CurrentMusicAudioComponent->FadeIn(FadeIn.Duration, FadeIn.VolumeLevel, StartTime, FadeIn.Curve);
}

void AMusicSystem::PlayNewMusicAbruptly(USoundBase* NewMusic, const float StartTime)
{
    if (CurrentMusicAudioComponent)
    {
        CurrentMusicAudioComponent->Stop();  // auto-destroy of old music audio component
    }

    CurrentMusicAudioComponent = UGameplayStatics::CreateSound2D(GetWorld(), NewMusic);
    if (!CurrentMusicAudioComponent)
    {
        UE_LOG(LogMusicSystem, Warning, TEXT("PlayMusicWithFadeIn is failed because new AudioComponent could not be created"));
        return;
    }
    CurrentMusicAudioComponent->Play(StartTime);
}

bool AMusicSystem::IsPaused() const
{
    const auto WorldSettings = GetWorldSettings();
    if (!WorldSettings) return false;

    return WorldSettings->GetPauserPlayerState() != NULL;
}
