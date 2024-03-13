// A test task by KEFIR
#pragma once
#include "CoreMinimal.h"

#include "BaseGameSetting.generated.h"

//-----------------------------
//  Base GameSetting
//-----------------------------

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

