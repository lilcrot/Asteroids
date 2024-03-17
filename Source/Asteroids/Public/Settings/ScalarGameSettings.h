// A test task by KEFIR
#pragma once
#include "CoreMinimal.h"
#include "Settings/BaseGameSetting.h"

#include "ScalarGameSettings.generated.h"

//---------------------------------
//  Scalar GameSettings
//---------------------------------

enum EScalarDisplayFormat
{
    SDF_Number,
    SDF_Percent
};


UCLASS()
class UScalarGameSetting : public UBaseGameSetting
{
    GENERATED_BODY()

public:
    void AddGetter(const TFunction<float()>& Func);
    void AddSetter(const TFunction<void(const float)>& Func);

    void SetDisplayFormat(const EScalarDisplayFormat& NewFormat);
    EScalarDisplayFormat GetDisplayFormat() const;

    float GetCurrentValue() const;
    void SetValue(const float NewValue);

private:
    TFunction<float()> Getter;
    TFunction<void(const float)> Setter;

    EScalarDisplayFormat DisplayFormat;
};
