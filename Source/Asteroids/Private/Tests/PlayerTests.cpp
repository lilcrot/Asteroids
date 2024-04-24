// A test task by KEFIR
#if WITH_AUTOMATION_TESTS
#include "Tests/PlayerTests.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Tests/Utils/TestUtils.h"
#include "Tests/TestConstants.h"
#include "Player/PlayerSpacecraft.h"
#include "Kismet/GameplayStatics.h"
#include "Tests/Utils/JsonUtils.h"
#include "Asteroid.h"

using namespace Test;
using namespace TestConstants;

namespace
{
AAsteroid* Asteroid;
}

IMPLEMENT_COMPLEX_AUTOMATION_TEST(FPlayerCanKill, "Asteroids.Entities.Player",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CriticalPriority);

void FPlayerCanKill::GetTests(TArray<FString>& OutBeautifiedNames, TArray<FString>& OutTestCommands) const
{
    struct FTestData
    {
        FString TestName;
        FString JsonName;
    };

    const TArray<FTestData> TestData =  //
        {
            {"Player_KillAsteroidByFirstWeapon", "Player_KillAsteroidByFirstWeapon.json"},  //
            {"Player_KillAsteroidBySecondWeapon", "Player_KillAsteroidBySecondWeapon.json"},    //
        };

    for (const auto OneTestData : TestData)
    {
        OutBeautifiedNames.Add(OneTestData.TestName);
        OutTestCommands.Add(OneTestData.JsonName);
    }
}
bool FPlayerCanKill::RunTest(const FString& Parameters)
{
    AutomationOpenMap(TestPlayerMapName);
    auto* World = GetTestGameWorld();

    auto* MyPlayer = Cast<APlayerSpacecraft>(UGameplayStatics::GetPlayerPawn(World, 0));
    if (!TestNotNull("Player exists", MyPlayer)) return false;

    auto* PlayerController = UGameplayStatics::GetPlayerController(World, 0);
    if (!TestNotNull("Player controller exists", PlayerController)) return false;

    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(World, AAsteroid::StaticClass(), FoundActors);
    if (!TestTrue("On TestPlayerMapName should exists only one asteroid", FoundActors.Num() == 1)) return false;
    Asteroid = Cast<AAsteroid>(FoundActors[0]);

    const FString FileName = GetTestDataDir().Append(Parameters);
    FInputData InputData;
    if (!JsonUtils::ReadInputData(FileName, InputData)) return false;
    if (!TestTrue("Input data is not empty", InputData.Bindings.Num() > 0)) return false;

    MyPlayer->SetActorTransform(InputData.InitialTransform);
    PlayerController->SetControlRotation(InputData.InitialTransform.Rotator());

    ADD_LATENT_AUTOMATION_COMMAND(FSimulateInputActionsLatentCommand(World, Cast<UEnhancedInputComponent>(MyPlayer->InputComponent),
        Cast<UEnhancedPlayerInput>(PlayerController->PlayerInput), InputData.Bindings));
    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([&]() 
        { 
            TestTrue("Asteroid was destroyed by weapon", !IsValid(Asteroid));
            return true; 
        }));

    ADD_LATENT_AUTOMATION_COMMAND(FExitGameCommand);

    return true;
}

#endif
