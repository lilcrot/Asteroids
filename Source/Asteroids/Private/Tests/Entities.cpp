// A test task by KEFIR

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Misc/PathViews.h"
#include "Tests/Utils/TestUtils.h"
#include "Tests/TestConstants.h"
#include "Asteroid.h"
#include "Components/HealthComponent.h"

BEGIN_DEFINE_SPEC(FAsteroidsEntities, "Asteroids",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)
UWorld* World;
AAsteroid *BigAsteroid, *SmallAsteroid;
END_DEFINE_SPEC(FAsteroidsEntities)

using namespace Test;
using namespace TestConstants;

void FAsteroidsEntities::Define()
{
    Describe("Entities.Asteroid",
        [this]()
        {
            BeforeEach(
                [this]()
                {
                    AutomationOpenMap(TestEmptyMapName);
                    World = GetTestGameWorld();
                });
            It("CanBeSpawned",
                [this]()
                {
                    const auto* Big = CreateBlueprint<AAsteroid>(World, BigAsteroidBPName);
                    TestTrue("BigAsteroid from blueprint can be spawned", IsValid(Big));

                    const auto* Small = CreateBlueprint<AAsteroid>(World, SmallAsteroidBPName);
                    TestTrue("SmallAsteroid from blueprint can be spawned", IsValid(Small));

                    SpecCloseLevel(World);
                });
            It("AlwaysMove",
                [this]()
                {
                    const FVector InitialLocation = FVector::ZeroVector;
                    SmallAsteroid = CreateBlueprint<AAsteroid>(World, SmallAsteroidBPName);

                    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(0.15))
                    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
                        [&]()
                        {
                            TestTrueExpr(IsValid(SmallAsteroid));
                            if (!SmallAsteroid) return true;

                            TestTrue("Vector location is updating", InitialLocation != SmallAsteroid->GetActorLocation());

                            SpecCloseLevel(World);
                            return true;
                        }));
                });
            // TODO: implement automation test
            xIt("BigAsteroidMustSplitUpAfterHisDeath", [this]() { unimplemented(); });
        });
}

#endif