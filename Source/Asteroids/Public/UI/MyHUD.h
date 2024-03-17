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
    UVideoSettingsWidget* VideoSettingsWidget = nullptr;

    UPROPERTY(EditDefaultsOnly, Category = "SettingsMenu")
    TSubclassOf<UVideoSettingsWidget> VideoSettingsWidgetClass = nullptr;

    UPROPERTY(BlueprintReadOnly, Category = "SettingsMenu")
    UAudioSettingsWidget* AudioSettingsWidget = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SettingsMenu")
    TSubclassOf<UAudioSettingsWidget> AudioSettingsWidgetClass = nullptr;

private:
    void CollapseAllSettingsMenu();

    template <typename T>
    friend void OpenSettingWidget(AMyHUD* MyHud, T*& WidgetPtr, UClass* Class);
};
