// A test task by KEFIR
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/AudioComponent.h"

#include "MusicSystem.generated.h"

class USoundBase;

USTRUCT(Blueprintable, BlueprintType)
struct FMusicFadeInformation
{
    GENERATED_BODY()

    FMusicFadeInformation() = default;

    FMusicFadeInformation(const float InDuration, const float InVolumeLevel, const EAudioFaderCurve InCurve = EAudioFaderCurve::Linear)
    {
        Duration = InDuration;
        VolumeLevel = InVolumeLevel;
        Curve = InCurve;
    }

    /* How long it should take to reach the FadeVolumeLevel */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.05", Units = "s"))
    float Duration = 0.1f;

    /* The percentage of the AudioComponents's calculated volume to fade to */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0"))
    float VolumeLevel = 1.0f;

    /* The curve to use when interpolating between the old and new volume */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EAudioFaderCurve Curve = EAudioFaderCurve::Linear;
};

UCLASS(Blueprintable, BlueprintType)
class ASTEROIDS_API AMusicSystem : public AActor
{
    GENERATED_BODY()

public:
    AMusicSystem();

protected:
    virtual void BeginPlay() override;

    UFUNCTION(BlueprintCallable, Category = "MusicSystem")
    void PlayNewMusicAbruptly(USoundBase* NewMusic, const float StartTime = 0.0f);

    /*
        Note: play new music with smooth transitions(FadeOut & FadeIn) between old and new. 
        IMPORTANT: on pause smooth transitions don't work!

        @param FadeOut these properties will be used in function FadeOut for old music if it exists.
        @param FadeIn these properties will be used in function FadeIn for new music.
    */
    UFUNCTION(BlueprintCallable, Category = "MusicSystem")
    void PlayNewMusic(USoundBase* NewMusic, const FMusicFadeInformation FadeOut,  //
        const FMusicFadeInformation FadeIn, const float StartTime = 0.0f);        //

    /*
        Note: Play default music with settuped properties [DefaultMusicSound & DefaultMusicSoundFadeInInfo]
        @param FadeOut these properties will be used in function FadeOut for old music if it exists.
    */
    UFUNCTION(BlueprintCallable, Category = "MusicSystem")
    void PlayDefaultMusic(const FMusicFadeInformation FadeOut, const float StartTime = 0.0f);

    /* This sounds will be played first when this MusicSystem is spawned */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MusicSystem")
    TObjectPtr<USoundBase> DefaultMusicSound;

    /* These properties will be used when DefaultMusicSound will be played. */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MusicSystem")
    FMusicFadeInformation DefaultMusicSoundFadeInInfo{1.0f, 1.0f};

private:
    UPROPERTY()
    TObjectPtr<UAudioComponent> CurrentMusicAudioComponent;

    UFUNCTION()
    void PlayMusicWithFadeIn(USoundBase* NewMusic, const FMusicFadeInformation FadeIn, const float StartTime = 0.0f);
    FTimerHandle OldMusicFadeOutDelayTimerHandle;

    bool IsPaused() const;
};
