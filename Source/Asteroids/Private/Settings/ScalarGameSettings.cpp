#include "Settings/ScalarGameSettings.h"

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

void UScalarGameSetting::SetDisplayFormat(const EScalarDisplayFormat& NewFormat) 
{
    DisplayFormat = NewFormat;
}

EScalarDisplayFormat UScalarGameSetting::GetDisplayFormat() const
{
    return DisplayFormat;
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