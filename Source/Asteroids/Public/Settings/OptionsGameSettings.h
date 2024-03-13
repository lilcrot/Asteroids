// A test task by KEFIR
#pragma once
#include "CoreMinimal.h"
#include "AudioMixerBlueprintLibrary.h"
#include "Settings/BaseGameSetting.h"

#include "OptionsGameSettings.generated.h"

//-----------------------------
//  Base Option GameSetting
//-----------------------------

UCLASS()
class UBaseOptionGameSetting : public UBaseGameSetting
{
    GENERATED_BODY()

public:
    /* Empty the PossibleOptions and set to Options param */
    void SetPossibleOptions(const TArray<FText>& Options);

    const TArray<FText>& GetPossibleOptions() const;

public:
    virtual void ApplyNextOption();
    virtual void ApplyPrevOption();
    virtual int32 GetCurrentOptionIndex() const;

protected:
    TArray<FText> PossibleOptions;

    /*
        For properly work GetCurrentOptionIndex must be overriden.
        @return INDEX_NONE if unsuccessful
    */
    int32 GetNextOptionIndex() const;

    /*
        For properly work GetCurrentOptionIndex must be overriden.
        @return INDEX_NONE if unsuccessful
    */
    int32 GetPrevOptionIndex() const;
};

//---------------------------------
//  Graphic & Video GameSettings
//---------------------------------

UCLASS()
class UVideoGameSetting : public UBaseOptionGameSetting
{
    GENERATED_BODY()

public:
    /* Note: Function must be set before object using */
    void AddGetter(const TFunction<int32()>& Func);
    /* Note: Function must be set before object using */
    void AddSetter(const TFunction<void(const int32)>& Func);

    virtual void ApplyNextOption() override;
    virtual void ApplyPrevOption() override;

    virtual int32 GetCurrentOptionIndex() const override;

private:
    int32 GetCurrentValue() const;

    TFunction<int32()> Getter;
    TFunction<void(const int32)> Setter;

    void SetCurrentValue(const int32 NewValue);
};

//------------------------------------
//  Audio Device Output GameSetting
//------------------------------------

DECLARE_MULTICAST_DELEGATE(FOnAudioSettingsUpdated);

UCLASS()
class UAudioDeviceOutputGameSetting : public UBaseOptionGameSetting
{
    GENERATED_BODY()
public:
    friend class UMyGameUserSettings;

    /* Note: Function must be set before object using */
    void AddGetter(const TFunction<FString()>& Func);
    /* Note: Function must be set before object using */
    void AddSetter(const TFunction<void(const FString&)>& Func);

    virtual void ApplyNextOption() override;
    virtual void ApplyPrevOption() override;

public:
    UFUNCTION()
    void OnAudioOutputDevicesObtained(const TArray<FAudioOutputDeviceInfo>& AvailableDevices);

    UFUNCTION()
    void DeviceAdded(const FString DeviceId);
    UFUNCTION()
    void DeviceRemoved(const FString DeviceId);

    UFUNCTION()
    void DefaultDeviceChanged(const EAudioDeviceChangedRole InRole, const FString DeviceId);

    virtual int32 GetCurrentOptionIndex() const override;

    FOnAudioSettingsUpdated OnAudioSettingsUpdated;

private:
    void OnInitialized();

    TFunction<FString()> Getter;
    TFunction<void(const FString&)> Setter;

    TArray<FAudioOutputDeviceInfo> OutputDevices;
    FOnAudioOutputDevicesObtained DevicesObtainedCallback;

    void SetAudioOutputDeviceId(const FString NewId);
    FString GetCurrentAudioOutputDeviceID() const;
};