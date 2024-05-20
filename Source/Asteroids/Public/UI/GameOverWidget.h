// A test task by KEFIR
#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "GameOverWidget.generated.h"

UCLASS()
class ASTEROIDS_API UGameOverWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UGameOverWidget(const FObjectInitializer& ObjectInitializer);
};
