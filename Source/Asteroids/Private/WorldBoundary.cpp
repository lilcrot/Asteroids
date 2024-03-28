// A test task by KEFIR
#include "WorldBoundary.h"
#include "Components/BoxComponent.h"
#include "AsteroidCoreTypes.h"

AWorldBoundary::AWorldBoundary()
{
    PrimaryActorTick.bCanEverTick = false;

    DeathCollision = CreateDefaultSubobject<UBoxComponent>("DeathCollision");
    checkf(DeathCollision, TEXT("DeathCollision doesn't exist!"));
    DeathCollision->SetCollisionProfileName(WorldBoundaryCollisionProfileName, true);

    SetRootComponent(DeathCollision);
}

void AWorldBoundary::BeginPlay()
{
    Super::BeginPlay();

    DeathCollision->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnDeathCollisionEndOverlap);
}

void AWorldBoundary::OnDeathCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent,  //
    AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)              //
{
    UWorld* World = GetWorld();
    if (World == nullptr) return;

    World->DestroyActor(OtherActor);
}