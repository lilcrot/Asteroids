// A test task by KEFIR
#include "WorldBoundary.h"
#include "Components/BoxComponent.h"
#include "AsteroidCoreTypes.h"

AWorldBoundary::AWorldBoundary()
{
    PrimaryActorTick.bCanEverTick = false;

    DeathCollision = CreateDefaultSubobject<UBoxComponent>("DeathCollision");
    DeathCollision->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName, true);

    SetRootComponent(DeathCollision);
}

void AWorldBoundary::BeginPlay()
{
    Super::BeginPlay();

    checkf(DeathCollision, TEXT("DeathCollision doesn't exist!"));

    DeathCollision->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnDeathCollisionEndOverlap);
}

void AWorldBoundary::OnDeathCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent,  //
    AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)             //
{
    if (!OtherActor || !GetWorld()) return;

    GetWorld()->DestroyActor(OtherActor);
}