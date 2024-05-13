// A test task by KEFIR

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Tests/Utils/TestUtils.h"
#include "Tests/TestConstants.h"
#include "Misc/PathViews.h"
#include "Settings/MyGameUserSettings.h"
#include "EnemyWavesGameModeBase.h"
#include "Player/PlayerSpacecraft.h"

BEGIN_DEFINE_SPEC(FAsteroidsGame, "Asteroids",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)
UWorld* World;
UMyGameUserSettings* GameUserSettings;
END_DEFINE_SPEC(FAsteroidsGame)

void FAsteroidsGame::Define()
{
    using namespace Test;
    using namespace TestConstants;

    Describe("Game",
        [this]()
        {
            It("GameMapShouldExist",
                [this]()
                {
                    const TArray<FString> GameMaps = {"M_Space", "M_TestEmpty", "M_MainMenu"};

                    TArray<FString> AllProjectMaps;
                    IFileManager::Get().FindFilesRecursive(AllProjectMaps, *FPaths::ProjectContentDir(), TEXT("*.umap"), true, false);

                    for (const auto& Map : GameMaps)
                    {
                        const bool bIsMapExists = AllProjectMaps.ContainsByPredicate(
                            [&](const FString& ProjectMap)
                            {
                                FStringView OutPath, OutName, OutExt;
                                FPathViews::Split(FStringView(ProjectMap), OutPath, OutName, OutExt);
                                return Map.Equals(FString(OutName));
                            });

                        const FString What = Map + FString(" might exists");
                        TestTrue(What, bIsMapExists);
                    }
                });
        });

    Describe("Game",
        [this]()
        {
            BeforeEach(
                [this]()
                {
                    AutomationOpenMap(MainGameMapName);
                    World = GetTestGameWorld();
                });
            It("ClassesShouldBeSetupCorrectly",
                [this]()
                {
                    TestNotNull("Asteroids game mode set up", Cast<AEnemyWavesGameModeBase>(World->GetAuthGameMode()));

                    const auto* FirstPlayerController = World->GetFirstPlayerController();
                    TestNotNull("FirstPlayerController exists", FirstPlayerController);
                    if (FirstPlayerController)
                    {
                        TestNotNull("Asteroids pawn set up", Cast<APlayerSpacecraft>(FirstPlayerController->GetPawn()));
                    }

                    TestNotNull("Asteroids game user settings set up", UMyGameUserSettings::Get());
                });
        });

    Describe("Game.GameUserSettings",
        [this]()
        {
            BeforeEach(
                [this]()
                {
                    AutomationOpenMap(TestEmptyMapName);
                    World = GetTestGameWorld();

                    GameUserSettings = UMyGameUserSettings::Get();
                });
            AfterEach([this]() { SpecCloseLevel(World); });

            It("SettingsExist",
                [this]()
                {
                    TestTrue("VideoSettings exist", GameUserSettings->GetVideoSettings().Num() != 0);
                    TestTrue("AudioSettings exist", GameUserSettings->GetAudioSettings().Num() != 0);

                    const auto* FirstPlayerController = World->GetFirstPlayerController();
                    TestTrueExpr(IsValid(FirstPlayerController));
                    if (FirstPlayerController)
                    {
                        TestTrue("SoundSettings exist",  //
                            GameUserSettings->GetSoundSettings(FirstPlayerController->GetLocalPlayer()).Num() != 0);
                    }
                });
        });
}

#endif