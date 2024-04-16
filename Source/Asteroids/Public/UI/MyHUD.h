// A test task by KEFIR
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"

#include "MyHUD.generated.h"

class UAudioSettingsWidget;
class UVideoSettingsWidget;

UCLASS()
class ASTEROIDS_API AMyHUD : public AHUD
{
    GENERATED_BODY()

protected:
    virtual void BeginPlay() override;

public:
    //----------------------
    //  Settings Menu
    //----------------------
    UFUNCTION(BlueprintCallable, Category = "SettingsMenu")
    void OpenVideoSettings();

    UFUNCTION(BlueprintCallable, Category = "SettingsMenu")
    void OpenAudioSettings();

protected:
    UPROPERTY(BlueprintReadOnly, Category = "SettingsMenu")
    TObjectPtr<UVideoSettingsWidget> VideoSettingsWidget;

    UPROPERTY(EditDefaultsOnly, Category = "SettingsMenu")
    TSubclassOf<UVideoSettingsWidget> VideoSettingsWidgetClass;

    UPROPERTY(BlueprintReadOnly, Category = "SettingsMenu")
    TObjectPtr<UAudioSettingsWidget> AudioSettingsWidget;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SettingsMenu")
    TSubclassOf<UAudioSettingsWidget> AudioSettingsWidgetClass;

private:
    void CollapseAllSettingsMenu();

    template <typename T>
    friend void OpenSettingWidget(AMyHUD* MyHud, TObjectPtr<T>& WidgetPtr, UClass* Class);
};
