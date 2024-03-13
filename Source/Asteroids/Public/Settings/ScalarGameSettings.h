// A test task by KEFIR
#pragma once
#include "CoreMinimal.h"
#include "Settings/BaseGameSetting.h"

#include "ScalarGameSettings.generated.h"

//---------------------------------
//  Scalar GameSettings
//---------------------------------

UCLASS()
class UScalarGameSetting : public UBaseGameSetting
{
    GENERATED_BODY()

public:
    /* Note: Function must be set before object using */
    void AddGetter(const TFunction<float()>& Func);
    /* Note: Function must be set before object using */
    void AddSetter(const TFunction<void(const float)>& Func);

    float GetCurrentValue() const;
    void SetValue(const float NewValue);

private:
    TFunction<float()> Getter;
    TFunction<void(const float)> Setter;
};
