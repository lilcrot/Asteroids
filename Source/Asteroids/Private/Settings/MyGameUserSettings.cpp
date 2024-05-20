// A test task by KEFIR
#include "Settings/MyGameUserSettings.h"
#include "Settings/ScalarGameSettings.h"
#include "Settings/OptionsGameSettings.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerInput.h"

DEFINE_LOG_CATEGORY_STATIC(LogMyGameUserSettings, All, All);

#define BIND_VIDEO_SETTING_FUNC(FUNC)                                                                                                      \
    [&](int32 Level)                                                                                                                       \
    {                                                                                                                                      \
        FUNC(Level);                                                                                                                       \
        ApplySettings(false);                                                                                                              \
    }

#define LOCTEXT_NAMESPACE "GameUserSettings"

UMyGameUserSettings::UMyGameUserSettings() {}

UMyGameUserSettings* UMyGameUserSettings::Get()
{
    return GEngine ? Cast<UMyGameUserSettings>(GEngine->GetGameUserSettings()) : nullptr;
}

UWorld* UMyGameUserSettings::GetWorld() const
{
    return CurrentWorld ? CurrentWorld : Super::GetWorld();
}

void UMyGameUserSettings::OnGameStarted(UWorld* World)
{
    CurrentWorld = World;
    checkf(CurrentWorld, TEXT("OnGameStarted: world isn't valid!"));
    FWorldDelegates::OnPostWorldInitialization.RemoveAll(this);
    FWorldDelegates::OnPostWorldInitialization.AddUObject(this, &ThisClass::OnPostWorldInitialization);

    // Set the volume of sounds from the config
    {
        const TArray<TPair<float, FSoundMixClassInfo>> SoundMixClasses{{OverallVolumePercentage, OverallAudioInfo},
            {MusicVolumePercentage, MusicAudioInfo}, {SoundFXVolumePercentage, SoundFXAudioInfo}};
        for (const auto& Pair : SoundMixClasses)
        {
            UGameplayStatics::SetSoundMixClassOverride(CurrentWorld, Pair.Value.SoundMix, Pair.Value.SoundClass, Pair.Key, 1.0f, 0.0f);
        }
    }
}

void UMyGameUserSettings::OnPostWorldInitialization(UWorld* World, const UWorld::InitializationValues IVS)
{
    CurrentWorld = World;
    checkf(CurrentWorld, TEXT("OnPostWorldInitialization: world isn't valid!"));
}

//------------------
//  VideoSettings
//------------------

const TArray<UVideoGameSetting*>& UMyGameUserSettings::GetVideoSettings(ULocalPlayer* InLocalPlayer)
{
    if (VideoSettings.IsEmpty())
    {
        InitializeVideoSettings(InLocalPlayer);
    }

    return VideoSettings;
}

void UMyGameUserSettings::InitializeVideoSettings(ULocalPlayer* InLocalPlayer)
{
    if (!InLocalPlayer)
    {
        UE_LOG(LogMyGameUserSettings, Error, TEXT("InitializeVideoSettings is failed because InLocalPlayer isn't valid!"));
        return;
    }

    const TArray<FText> VideoSettingOptions = {
        LOCTEXT("VideoSettingQualityLow_Loc", "Low"),        // 0 lvl
        LOCTEXT("VideoSettingQualityMedium_Loc", "Medium"),  // 1 lvl
        LOCTEXT("VideoSettingQualityHigh_Loc", "High"),      // 2 lvl
        LOCTEXT("VideoSettingQualityEpic_Loc", "Epic")       // 3 lvl
    };

    const auto AddSetting = [&](const FText& Name, TFunction<int32()> Getter, TFunction<void(const int32)> Setter)
    {
        auto* Setting = NewObject<UVideoGameSetting>(InLocalPlayer);
        check(Setting);
        Setting->SetName(Name);
        Setting->SetPossibleOptions(VideoSettingOptions);
        Setting->AddGetter(Getter);
        Setting->AddSetter(Setter);
        VideoSettings.Add(Setting);
    };

    // clang-format off

        AddSetting(LOCTEXT("AntiAliasing_Loc", "Anti-Aliasing"), [&]() { return GetAntiAliasingQuality(); }, BIND_VIDEO_SETTING_FUNC(SetAntiAliasingQuality));
        AddSetting(LOCTEXT("Textures_Loc", "Textures"), [&]() { return GetTextureQuality(); }, BIND_VIDEO_SETTING_FUNC(SetTextureQuality));
        AddSetting(LOCTEXT("GlobalIllumination_Loc", "Global Illumination"), [&]() { return GetGlobalIlluminationQuality(); }, BIND_VIDEO_SETTING_FUNC(SetGlobalIlluminationQuality));
        AddSetting(LOCTEXT("Shadows_Loc", "Shadows"), [&]() { return GetShadowQuality(); }, BIND_VIDEO_SETTING_FUNC(SetShadowQuality));
        AddSetting(LOCTEXT("PostProcessing_Loc", "Post Processing"), [&]() { return GetPostProcessingQuality(); }, BIND_VIDEO_SETTING_FUNC(SetPostProcessingQuality));
        AddSetting(LOCTEXT("Reflections_Loc", "Reflections"), [&]() { return GetReflectionQuality(); }, BIND_VIDEO_SETTING_FUNC(SetReflectionQuality));
        AddSetting(LOCTEXT("Effects_Loc", "Effects"), [&]() { return GetVisualEffectQuality(); }, BIND_VIDEO_SETTING_FUNC(SetVisualEffectQuality));

    // clang-format on
}

void UMyGameUserSettings::RunBenchmark()
{
    RunHardwareBenchmark();
    ApplySettings(false);
    OnVideoSettingsUpdated.Broadcast();
}

//------------------
//  AudioSettings
//------------------

const TArray<UScalarGameSetting*>& UMyGameUserSettings::GetAudioSettings(ULocalPlayer* InLocalPlayer)
{
    if (AudioSettings.IsEmpty())
    {
        InitializeAudioSettings(InLocalPlayer);
    }

    return AudioSettings;
}

void UMyGameUserSettings::InitializeAudioSettings(ULocalPlayer* InLocalPlayer)
{
    if (!InLocalPlayer)
    {
        UE_LOG(LogMyGameUserSettings, Error, TEXT("InitializeAudioSettings is failed because InLocalPlayer isn't valid!"));
        return;
    }
    const auto AddSetting =
        [&](const FText& Name, const EScalarDisplayFormat& Format, TFunction<float()> Getter, TFunction<void(const float)> Setter)
    {
        auto* Setting = NewObject<UScalarGameSetting>(InLocalPlayer);
        check(Setting);
        Setting->SetName(Name);
        Setting->AddGetter(Getter);
        Setting->AddSetter(Setter);
        Setting->SetDisplayFormat(Format);

        AudioSettings.Add(Setting);
    };

    // clang-format off
              
        AddSetting(LOCTEXT("OverallVolume_Loc", "OverallVolume"), EScalarDisplayFormat::SDF_Percent, [&]() { return OverallVolumePercentage; },
            [&](const float NewValue)
            {
                OverallVolumePercentage = FMath::Clamp(NewValue, 0.0f, 1.0f);
                UGameplayStatics::SetSoundMixClassOverride(GetWorld(), OverallAudioInfo.SoundMix, OverallAudioInfo.SoundClass, OverallVolumePercentage, 1.0f, 0.0f);
                ApplySettings(false);
            });

        AddSetting(LOCTEXT("MusicVolume_Loc", "MusicVolume"), EScalarDisplayFormat::SDF_Percent, [&]() { return MusicVolumePercentage; },
            [&](const float NewValue)
            {
                MusicVolumePercentage = FMath::Clamp(NewValue, 0.0f, 1.0f);
                UGameplayStatics::SetSoundMixClassOverride(GetWorld(), MusicAudioInfo.SoundMix, MusicAudioInfo.SoundClass, MusicVolumePercentage, 1.0f, 0.0f);
                ApplySettings(false);
            });


        AddSetting(LOCTEXT("SoundFXVolume_Loc", "SoundFXVolume"), EScalarDisplayFormat::SDF_Percent, [&]() { return SoundFXVolumePercentage; },
            [&](const float NewValue)
            {
                SoundFXVolumePercentage = FMath::Clamp(NewValue, 0.0f, 1.0f);
                UGameplayStatics::SetSoundMixClassOverride(GetWorld(), SoundFXAudioInfo.SoundMix, SoundFXAudioInfo.SoundClass, SoundFXVolumePercentage, 1.0f, 0.0f);
                ApplySettings(false);
            });

    // clang-format on
}

const TArray<UBaseGameSetting*>& UMyGameUserSettings::GetSoundSettings(ULocalPlayer* InLocalPlayer)
{
    if (SoundSettings.IsEmpty())
    {
        InitializeSoundSettings(InLocalPlayer);
    }

    return SoundSettings;
}

void UMyGameUserSettings::InitializeSoundSettings(ULocalPlayer* InLocalPlayer)
{
    if (!InLocalPlayer)
    {
        UE_LOG(LogMyGameUserSettings, Error, TEXT("InitializeSoundSettings is failed because InLocalPlayer isn't valid!"));
        return;
    }
    // clang-format off

    /*-------------------------*/
    auto* AudioDeviceOutputGameSetting = NewObject<UAudioDeviceOutputGameSetting>(InLocalPlayer);
    checkf(IsValid(AudioDeviceOutputGameSetting), TEXT("AudioDeviceOutputGameSetting isn't valid!"));

    AudioDeviceOutputGameSetting->SetName(LOCTEXT("AudioOutputDevice_Loc", "AudioOutputDevice"));
    AudioDeviceOutputGameSetting->AddGetter([&]() { return AudioOutputDeviceId; });
    AudioDeviceOutputGameSetting->AddSetter(
        [&](const FString& InAudioOutputDeviceId)
        {
            AudioOutputDeviceId = InAudioOutputDeviceId;

            FOnCompletedDeviceSwap DevicesSwappedCallback;
            DevicesSwappedCallback.BindUFunction(this, FName("OnCompletedAudioDeviceSwap"));
            UAudioMixerBlueprintLibrary::SwapAudioOutputDevice(GetWorld(), InAudioOutputDeviceId, DevicesSwappedCallback);

            ApplySettings(false);
        });
    AudioDeviceOutputGameSetting->OnInitialized();

    SoundSettings.AddUnique(AudioDeviceOutputGameSetting);

#if WITH_EDITOR
    FWorldDelegates::OnWorldCleanup.RemoveAll(this);
    FWorldDelegates::OnWorldCleanup.AddUObject(this, &ThisClass::OnWorldCleanup);
#endif  // WITH_EDITOR

    /*-------------------------*/

    // clang-format on
}

void UMyGameUserSettings::OnCompletedAudioDeviceSwap(const FSwapAudioOutputResult& SwapResult)
{
    if (SwapResult.Result == ESwapAudioOutputDeviceResultState::Failure)
    {
        UE_LOG(LogMyGameUserSettings, Error,                                                                    //
            TEXT("SwapAudioOutputDevice is failed, RequestedDeviceId is %s!"), *SwapResult.RequestedDeviceId);  //
        return;
    }
}

#if WITH_EDITOR
void UMyGameUserSettings::OnWorldCleanup(UWorld* World, const bool bSessionEnded, const bool bCleanupResources)
{
    if (bSessionEnded)
    {
        /* The SoundSettings are dependent on the WorldContext and can be changed in the editor, so we need to handle this case for
         * With_EDITOR. */
        SoundSettings.Empty();
    }
}
#endif  // WITH_EDITOR

#undef LOCTEXT_NAMESPACE