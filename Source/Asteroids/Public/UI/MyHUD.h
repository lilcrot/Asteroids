// A test task by KEFIR
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"

#include "MyHUD.generated.h"

class UAudioSettingsWidget;
class UVideoSettingsWidget;
class UPauseMenuWidget;
class UGameplayWidget;

UCLASS()
class ASTEROIDS_API AMyHUD : public AHUD
{
    GENERATED_BODY()

protected:
    virtual void BeginPlay() override;

    // @param Class: if the NewWidget does not exist, it will be created using this class
    template <typename T>
    friend void SetNewCurrentWidget(AMyHUD* MyHud, TObjectPtr<T>& NewWidget, TSubclassOf<T> Class);


private:
    TObjectPtr<UUserWidget> CurrentWidget;

protected:
    //----------------------
    //  GameInProgress
    //----------------------

    UPROPERTY(BlueprintReadOnly, Category = "UI|GameInProgress")
    TObjectPtr<UGameplayWidget> GameplayWidget;
    UPROPERTY(EditDefaultsOnly, Category = "UI|GameInProgress")
    TSubclassOf<UGameplayWidget> GameplayWidgetClass;

    UFUNCTION(BlueprintCallable, Category = "UI|GameInProgress")
    void OpenGameplayWidget();

public:
    //----------------------
    //  GameInPause
    //----------------------
    UFUNCTION(BlueprintCallable, Category = "UI|GameInPause")
    void OpenVideoSettings();

    UFUNCTION(BlueprintCallable, Category = "UI|GameInPause")
    void OpenAudioSettings();

private:
    UFUNCTION()
    void OnGamePauseChanged(const bool bIsPaused);

protected:
    UPROPERTY(BlueprintReadOnly, Category = "UI|GameInPause")
    TObjectPtr<UPauseMenuWidget> PauseMenuWidget;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI|GameInPause")
    TSubclassOf<UPauseMenuWidget> PauseMenuWidgetClass;

    UPROPERTY(BlueprintReadOnly, Category = "UI|GameInPause")
    TObjectPtr<UVideoSettingsWidget> VideoSettingsWidget;
    UPROPERTY(EditDefaultsOnly, Category = "UI|GameInPause")
    TSubclassOf<UVideoSettingsWidget> VideoSettingsWidgetClass;

    UPROPERTY(BlueprintReadOnly, Category = "UI|GameInPause")
    TObjectPtr<UAudioSettingsWidget> AudioSettingsWidget;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI|GameInPause")
    TSubclassOf<UAudioSettingsWidget> AudioSettingsWidgetClass;

private:
    template <typename T>
    friend void OpenGameInPauseWidget(AMyHUD* MyHud, TObjectPtr<T>& WidgetPtr, TSubclassOf<T> Class);
};
