// A test task by KEFIR
#include "Settings/MyGameUserSettings.h"

#include "Settings/ScalarGameSettings.h"
#include "Settings/OptionsGameSettings.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerInput.h"

#define BIND_VIDEO_SETTING_FUNC(FUNC)                                                                                                      \
    [&](int32 Level)                                                                                                                       \
    {                                                                                                                                      \
        FUNC(Level);                                                                                                                       \
        ApplySettings(false);                                                                                                              \
    }

#define LOCTEXT_NAMESPACE "GameUserSettings"

UMyGameUserSettings::UMyGameUserSettings()
{
    InitializeVideoSettings();
    InitializeAudioSettings();
}

UMyGameUserSettings* UMyGameUserSettings::Get()
{
    return GEngine ? Cast<UMyGameUserSettings>(GEngine->GetGameUserSettings()) : nullptr;
}

//------------------
//  VideoSettings
//------------------

const TArray<UVideoGameSetting*>& UMyGameUserSettings::GetVideoSettings() const
{
    return VideoSettings;
}

void UMyGameUserSettings::RunBenchmark()
{
    RunHardwareBenchmark();
    ApplySettings(false);
    OnVideoSettingsUpdated.Broadcast();
}

void UMyGameUserSettings::InitializeVideoSettings()
{
    const TArray<FText> VideoSettingOptions = {
        LOCTEXT("VideoSettingQualityLow_Loc", "Low"),        // 0 lvl
        LOCTEXT("VideoSettingQualityMedium_Loc", "Medium"),  // 1 lvl
        LOCTEXT("VideoSettingQualityHigh_Loc", "High"),      // 2 lvl
        LOCTEXT("VideoSettingQualityEpic_Loc", "Epic")       // 3 lvl
    };

    const auto AddSetting = [&](const FText& Name, TFunction<int32()> Getter, TFunction<void(const int32)> Setter)
    {
        auto* Setting = NewObject<UVideoGameSetting>();
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

//------------------
//  AudioSettings
//------------------

const TArray<UScalarGameSetting*>& UMyGameUserSettings::GetAudioSettings() const
{
    return AudioSettings;
}

const TArray<UBaseGameSetting*>& UMyGameUserSettings::GetSoundSettings(ULocalPlayer* InLocalPlayer)
{
    // clang-format off
    if (SoundSettings.IsEmpty() && IsValid(InLocalPlayer))
    {
        /*-------------------------*/
        auto* AudioDeviceOutputGameSetting = NewObject<UAudioDeviceOutputGameSetting>(InLocalPlayer);
        checkf(IsValid(AudioDeviceOutputGameSetting), TEXT("AudioDeviceOutputGameSetting isn't valid!"));

        AudioDeviceOutputGameSetting->SetName(LOCTEXT("AudioOutputDevice_Loc", "AudioOutputDevice"));
        AudioDeviceOutputGameSetting->AddGetter([&]() { return AudioOutputDeviceId; });
        AudioDeviceOutputGameSetting->AddSetter(
            [&](const FString& InAudioOutputDeviceId)
            {
                AudioOutputDeviceId = InAudioOutputDeviceId;
                ApplySettings(false);
            });
        AudioDeviceOutputGameSetting->OnInitialized();

        SoundSettings.AddUnique(AudioDeviceOutputGameSetting);

        #if WITH_EDITOR
        FWorldDelegates::OnWorldCleanup.AddUObject(this, &ThisClass::OnWorldCleanup);
        #endif // WITH_EDITOR

        /*-------------------------*/

    }
    // clang-format on

    return SoundSettings;
}

void UMyGameUserSettings::InitializeAudioSettings()
{
    const auto AddSetting =
        [&](const FText& Name, const EScalarDisplayFormat& Format, TFunction<float()> Getter, TFunction<void(const float)> Setter)
    {
        auto* Setting = NewObject<UScalarGameSetting>();
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
            UGameplayStatics::SetSoundMixClassOverride(GetWorld(), OverrallAudioInfo.SoundMix, OverrallAudioInfo.SoundClass, OverallVolumePercentage);
            ApplySettings(false);
        });

    AddSetting(LOCTEXT("MusicVolume_Loc", "MusicVolume"), EScalarDisplayFormat::SDF_Percent, [&]() { return MusicVolumePercentage; },
        [&](const float NewValue)
        {
            MusicVolumePercentage = FMath::Clamp(NewValue, 0.0f, 1.0f);
            UGameplayStatics::SetSoundMixClassOverride(GetWorld(), MusicAudioInfo.SoundMix, MusicAudioInfo.SoundClass, MusicVolumePercentage);
            ApplySettings(false);
        });


    AddSetting(LOCTEXT("SoundFXVolume_Loc", "SoundFXVolume"), EScalarDisplayFormat::SDF_Percent, [&]() { return SoundFXVolumePercentage; },
        [&](const float NewValue)
        {
            SoundFXVolumePercentage = FMath::Clamp(NewValue, 0.0f, 1.0f);
            UGameplayStatics::SetSoundMixClassOverride(GetWorld(), SoundFXAudioInfo.SoundMix, SoundFXAudioInfo.SoundClass, SoundFXVolumePercentage);
            ApplySettings(false);
        });

    // clang-format on
}

#if WITH_EDITOR
void UMyGameUserSettings::OnWorldCleanup(UWorld* World, const bool bSessionEnded, const bool bCleanupResources)
{
    if (bSessionEnded)
    {
        /* The SoundSettings depend on the WorldContext, so we need to handle this case for WITH_EDITOR */
        SoundSettings.Empty();
    }
}
#endif  // WITH_EDITOR

#undef LOCTEXT_NAMESPACE