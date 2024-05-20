// A test task by KEFIR

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"

#include "MyGameUserSettings.generated.h"

class UBaseGameSetting;
class UVideoGameSetting;
class UScalarGameSetting;
class UAudioDeviceOutputGameSetting;

class USoundClass;
class USoundMix;

USTRUCT(BlueprintType)
struct FSoundMixClassInfo
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere)
    TObjectPtr<USoundMix> SoundMix;

    UPROPERTY(EditAnywhere)
    TObjectPtr<USoundClass> SoundClass;
};

DECLARE_MULTICAST_DELEGATE(FOnSettingsUpdatedDelegate);

UCLASS(Abstract, BlueprintType, Blueprintable)
class ASTEROIDS_API UMyGameUserSettings : public UGameUserSettings
{
    GENERATED_BODY()
    friend class UMyGameInstance;

public:
    UMyGameUserSettings();

    static UMyGameUserSettings* Get();

    const TArray<UVideoGameSetting*>& GetVideoSettings(ULocalPlayer* InLocalPlayer);
    const TArray<UScalarGameSetting*>& GetAudioSettings(ULocalPlayer* InLocalPlayer);
    const TArray<UBaseGameSetting*>& GetSoundSettings(ULocalPlayer* InLocalPlayer);

    virtual class UWorld* GetWorld() const;

private:
    UPROPERTY()
    TObjectPtr<UWorld> CurrentWorld;

    void OnGameStarted(UWorld* World);  // called by MyGameInstance
    void OnPostWorldInitialization(UWorld* World, const UWorld::InitializationValues IVS);

public:
    //------------------
    //  VideoSettings
    //------------------

    /* Sets the most optimal video settings for your system */
    void RunBenchmark();

    FOnSettingsUpdatedDelegate OnVideoSettingsUpdated;

private:
    UPROPERTY()
    TArray<TObjectPtr<UVideoGameSetting>> VideoSettings;
    void InitializeVideoSettings(ULocalPlayer* InLocalPlayer);

public:
    //--------------------------
    //  Audio & Sound Settings
    //--------------------------

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AudioSettings")
    FSoundMixClassInfo OverallAudioInfo;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AudioSettings")
    FSoundMixClassInfo MusicAudioInfo;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AudioSettings")
    FSoundMixClassInfo SoundFXAudioInfo;

private:
    UPROPERTY()
    TArray<TObjectPtr<UScalarGameSetting>> AudioSettings;
    void InitializeAudioSettings(ULocalPlayer* InLocalPlayer);

    UPROPERTY()
    TArray<TObjectPtr<UBaseGameSetting>> SoundSettings;
    void InitializeSoundSettings(ULocalPlayer* InLocalPlayer);

#if WITH_EDITOR
    void OnWorldCleanup(UWorld* World, const bool bSessionEnded, const bool bCleanupResources);
#endif  // WITH_EDITOR

    UPROPERTY(Config)
    float OverallVolumePercentage = 1.0f;

    UPROPERTY(Config)
    float MusicVolumePercentage = 1.0f;

    UPROPERTY(Config)
    float SoundFXVolumePercentage = 1.0f;

    UPROPERTY(Config)
    FString AudioOutputDeviceId;

    UFUNCTION()
    void OnCompletedAudioDeviceSwap(const FSwapAudioOutputResult& SwapResult);
};