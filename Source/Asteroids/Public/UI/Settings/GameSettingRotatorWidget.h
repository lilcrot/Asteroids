// A test task by KEFIR

#pragma once

#include "CoreMinimal.h"
#include "CommonRotator.h"
#include "GameSettingRotatorWidget.generated.h"

UCLASS(Abstract, meta = (Category = "Settings", DisableNativeTick))
class ASTEROIDS_API UGameSettingRotatorWidget : public UCommonRotator
{
    GENERATED_BODY()

public:
    UGameSettingRotatorWidget(const FObjectInitializer& Initializer);

    // Note: INDEX_NONE(-1) will hide default option
    UFUNCTION(BlueprintCallable)
    void SetDefaultOption(const int32 DefaultOptionIndex);

protected:
    UFUNCTION(BlueprintImplementableEvent, Category = "Events", meta = (DisplayName = "On Default Option Specified"))
    void OnDefaultOptionSpecified(const int32 DefaultOptionIndex);
};
