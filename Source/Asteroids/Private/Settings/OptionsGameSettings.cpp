#include "Settings/OptionsGameSettings.h"

#include "AudioDeviceNotificationSubsystem.h"
#include "AudioMixerBlueprintLibrary.h"

#define LOCTEXT_NAMESPACE "GameUserSettings"

//-----------------------------
//  Base Option GameSetting
//-----------------------------

void UBaseOptionGameSetting::SetPossibleOptions(const TArray<FText>& Options)
{
    PossibleOptions = Options;
}

const TArray<FText>& UBaseOptionGameSetting::GetPossibleOptions() const
{
    return PossibleOptions;
}

int32 UBaseOptionGameSetting::GetNextOptionIndex() const
{
    const int32 CurrentIndex = GetCurrentOptionIndex();
    if (CurrentIndex == INDEX_NONE) return INDEX_NONE;

    return (CurrentIndex + 1) % PossibleOptions.Num();
}

int32 UBaseOptionGameSetting::GetPrevOptionIndex() const
{
    const int32 CurrentIndex = GetCurrentOptionIndex();
    if (CurrentIndex == INDEX_NONE) return INDEX_NONE;

    return CurrentIndex == 0 ? PossibleOptions.Num() - 1 : CurrentIndex - 1;
}

void UBaseOptionGameSetting::ApplyNextOption() {}

void UBaseOptionGameSetting::ApplyPrevOption() {}

int32 UBaseOptionGameSetting::GetCurrentOptionIndex() const
{
    return INDEX_NONE;
}

//---------------------------------
//  Graphic & Video GameSettings
//---------------------------------

DEFINE_LOG_CATEGORY_STATIC(LogVideoGameSetting, All, All);

void UVideoGameSetting::AddGetter(const TFunction<int32()>& Func)
{
    Getter = Func;
}

void UVideoGameSetting::AddSetter(const TFunction<void(const int32)>& Func)
{
    Setter = Func;
}

void UVideoGameSetting::ApplyNextOption()
{
    const int32 NextIndex = GetNextOptionIndex();
    if (PossibleOptions.IsValidIndex(NextIndex))
    {
        SetCurrentValue(NextIndex);
    }
}
void UVideoGameSetting::ApplyPrevOption()
{
    const int32 PrevIndex = GetNextOptionIndex();
    if (PossibleOptions.IsValidIndex(PrevIndex))
    {
        SetCurrentValue(PrevIndex);
    }
}

int32 UVideoGameSetting::GetCurrentOptionIndex() const
{
    return GetCurrentValue();
}

int32 UVideoGameSetting::GetCurrentValue() const
{
    if (!Getter)
    {
        UE_LOG(LogVideoGameSetting, Error, TEXT("Getter func is not set for %s"), *GetName().ToString());
        return INDEX_NONE;
    }
    return Getter();
}

void UVideoGameSetting::SetCurrentValue(const int32 NewValue)
{
    if (!Setter)
    {
        UE_LOG(LogVideoGameSetting, Error, TEXT("Setter func is not set for %s"), *GetName().ToString());
        return;
    }
    Setter(NewValue);
}

//------------------------------------
//  Audio Device Output GameSetting
//------------------------------------

DEFINE_LOG_CATEGORY_STATIC(LogAudioDeviceOutputGameSetting, All, All);

void UAudioDeviceOutputGameSetting::OnInitialized()
{
    DevicesObtainedCallback.BindUFunction(this, FName("OnAudioOutputDevicesObtained"));

    if (auto* AudioDeviceNotifSubsystem = UAudioDeviceNotificationSubsystem::Get())
    {
        AudioDeviceNotifSubsystem->DeviceAddedNative.AddUObject(this, &ThisClass::DeviceAdded);
        AudioDeviceNotifSubsystem->DeviceRemovedNative.AddUObject(this, &ThisClass::DeviceRemoved);

        AudioDeviceNotifSubsystem->DefaultRenderDeviceChangedNative.AddUObject(this, &ThisClass::DefaultDeviceChanged);
    }

    UAudioMixerBlueprintLibrary::GetAvailableAudioOutputDevices(this, DevicesObtainedCallback);
}

void UAudioDeviceOutputGameSetting::SetAudioOutputDeviceId(const FString NewId)
{
    if (!Setter)
    {
        UE_LOG(LogAudioDeviceOutputGameSetting, Error, TEXT("Setter func is not set for %s"), *GetName().ToString());
        return;
    }
    Setter(NewId);
}

FString UAudioDeviceOutputGameSetting::GetCurrentAudioOutputDeviceID() const
{
    if (!Getter)
    {
        UE_LOG(LogAudioDeviceOutputGameSetting, Error, TEXT("Getter func is not set for %s"), *GetName().ToString());
        return FString();
    }
    return Getter();
}

int32 UAudioDeviceOutputGameSetting::GetCurrentOptionIndex() const
{
    const FString CurrentID = GetCurrentAudioOutputDeviceID();
    return OutputDevices.IndexOfByPredicate([&](const FAudioOutputDeviceInfo& Info) { return Info.DeviceId == CurrentID; });
}

void UAudioDeviceOutputGameSetting::AddGetter(const TFunction<FString()>& Func)
{
    Getter = Func;
}

void UAudioDeviceOutputGameSetting::AddSetter(const TFunction<void(const FString&)>& Func)
{
    Setter = Func;
}

void UAudioDeviceOutputGameSetting::ApplyNextOption()
{
    const int32 NextIndex = GetNextOptionIndex();
    if (OutputDevices.IsValidIndex(NextIndex))
    {
        SetAudioOutputDeviceId(OutputDevices[NextIndex].DeviceId);
    }
}
void UAudioDeviceOutputGameSetting::ApplyPrevOption()
{
    const int32 PrevIndex = GetNextOptionIndex();
    if (OutputDevices.IsValidIndex(PrevIndex))
    {
        SetAudioOutputDeviceId(OutputDevices[PrevIndex].DeviceId);
    }
}

void UAudioDeviceOutputGameSetting::OnAudioOutputDevicesObtained(const TArray<FAudioOutputDeviceInfo>& AvailableDevices)
{
    PossibleOptions.Empty();
    OutputDevices.Empty();

    const FString SelectedAudioOutputDeviceID = GetCurrentAudioOutputDeviceID();
    const auto* SelectedAudioOutputDevice =
        AvailableDevices.FindByPredicate([&](const FAudioOutputDeviceInfo& Info) { return Info.DeviceId == SelectedAudioOutputDeviceID; });
    
    const bool bHaveSelectedOutDevice = SelectedAudioOutputDeviceID.IsEmpty() == false && SelectedAudioOutputDevice != nullptr;

    for (const auto& DeviceInfo : AvailableDevices)
    {
        if (DeviceInfo.DeviceId.IsEmpty() || DeviceInfo.Name.IsEmpty()) continue;

        if (bHaveSelectedOutDevice == false)
        {
            if (DeviceInfo.bIsSystemDefault)
            {
                SetAudioOutputDeviceId(DeviceInfo.DeviceId);
            }
            if (DeviceInfo.bIsCurrentDevice)
            {
                SetAudioOutputDeviceId(DeviceInfo.DeviceId);
            }
        }

        OutputDevices.Add(DeviceInfo);
        PossibleOptions.Add(FText::FromString(DeviceInfo.Name));
    }
    OnAudioSettingsUpdated.Broadcast();
}

void UAudioDeviceOutputGameSetting::DeviceAdded(const FString DeviceId)
{
    UAudioMixerBlueprintLibrary::GetAvailableAudioOutputDevices(this, DevicesObtainedCallback);
}
void UAudioDeviceOutputGameSetting::DeviceRemoved(const FString DeviceId)
{
    UAudioMixerBlueprintLibrary::GetAvailableAudioOutputDevices(this, DevicesObtainedCallback);
}

void UAudioDeviceOutputGameSetting::DefaultDeviceChanged(const EAudioDeviceChangedRole InRole, const FString DeviceId)
{
    UAudioMixerBlueprintLibrary::GetAvailableAudioOutputDevices(this, DevicesObtainedCallback);
}

#undef LOCTEXT_NAMESPACE
