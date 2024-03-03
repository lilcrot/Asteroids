// A test task by KEFIR

#pragma once

#include "CoreMinimal.h"
#include "GameSettings.generated.h"

UCLASS(NotBlueprintable, NotBlueprintType)
class UBaseGameSetting : public UObject
{
    GENERATED_BODY()

public:
    void SetName(const FText& InName);
    FText GetName() const;

private:
    FText Name;
};



//---------------------------------
//  Graphic & Video GameSettings
//---------------------------------


struct FSettingOption
{
    /* Low, Medium, High... */
    FText QualityName;
    int32 Value = 0;
};

UCLASS()
class UVideoGameSetting : public UBaseGameSetting
{
    GENERATED_BODY()

public:
    void SetOptions(const TArray<FSettingOption>& Options);

    /* @Note Function must be set before object using */
    void AddGetter(const TFunction<int32()>& Func);
    /* @Note Function must be set before object using */
    void AddSetter(const TFunction<void(const int32)>& Func);

    void ApplyNextOption();
    void ApplyPrevOption();

    int32 GetCurrentOptionIndex() const;

    const TArray<FSettingOption>& GetPossibleOptions() const;

private:
    TArray<FSettingOption> PossibleOptions;
    TFunction<int32()> Getter;
    TFunction<void(const int32)> Setter;

    int32 GetCurrentValue() const;
    void SetCurrentValue(const int32 NewValue);
};


//---------------------------------
//  Scalar GameSettings
//---------------------------------

UCLASS()
class UScalarGameSetting : public UBaseGameSetting
{
    GENERATED_BODY()

public:
    /* @Note Function must be set before object using */
    void AddGetter(const TFunction<float()>& Func);
    /* @Note Function must be set before object using */
    void AddSetter(const TFunction<void(const float)>& Func);

    float GetCurrentValue() const;
    void SetValue(const float NewValue);

private:
    TFunction<float()> Getter;
    TFunction<void(const float)> Setter;
};