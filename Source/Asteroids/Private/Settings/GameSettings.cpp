// A test task by KEFIR
#include "Settings/GameSettings.h"


void UBaseGameSetting::SetName(const FText& InName) 
{
    Name = InName;
}

FText UBaseGameSetting::GetName() const
{
    return Name;
}



//---------------------------------
//  Graphic & Video GameSettings
//---------------------------------

DEFINE_LOG_CATEGORY_STATIC(LogVideoGameSetting, All, All);

void UVideoGameSetting::SetOptions(const TArray<FSettingOption>& InOptions)
{
    PossibleOptions = InOptions;
}

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
    const int32 CurrentIndex = GetCurrentOptionIndex();
    if (CurrentIndex == INDEX_NONE) return;

    const int32 NextIndex = (CurrentIndex + 1) % PossibleOptions.Num();
    SetCurrentValue(PossibleOptions[NextIndex].Value);
}

void UVideoGameSetting::ApplyPrevOption()
{
    const int32 CurrentIndex = GetCurrentOptionIndex();
    if (CurrentIndex == INDEX_NONE) return;

    const int32 PrevIndex = CurrentIndex == 0 ? PossibleOptions.Num() - 1 : CurrentIndex - 1;
    SetCurrentValue(PossibleOptions[PrevIndex].Value);
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

int32 UVideoGameSetting::GetCurrentOptionIndex() const
{
    const int32 CurrentValue = GetCurrentValue();
    return PossibleOptions.IndexOfByPredicate([&](const auto& Opt) { return CurrentValue == Opt.Value; });
}

const TArray<FSettingOption>& UVideoGameSetting::GetPossibleOptions() const
{
    return PossibleOptions;
}


//---------------------------------
//  Scalar GameSettings
//---------------------------------

DEFINE_LOG_CATEGORY_STATIC(LogScalarGameSetting, All, All);

void UScalarGameSetting::AddGetter(const TFunction<float()>& Func)
{
    Getter = Func;
}

void UScalarGameSetting::AddSetter(const TFunction<void(const float)>& Func)
{
    Setter = Func;
}

float UScalarGameSetting::GetCurrentValue() const
{
    if (!Getter)
    {
        UE_LOG(LogScalarGameSetting, Error, TEXT("Getter func is not set for %s"), *GetName().ToString());
        return INDEX_NONE;
    }
    return Getter();
}

void UScalarGameSetting::SetValue(const float NewValue)
{
    if (!Setter)
    {
        UE_LOG(LogScalarGameSetting, Error, TEXT("Setter func is not set for %s"), *GetName().ToString());
        return;
    }
    Setter(NewValue);
}


