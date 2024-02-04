// A test task by KEFIR
#include "WorldObtacle.h"
#include "Components/BoxComponent.h"
#include "AsteroidCoreTypes.h"

AWorldObtacle::AWorldObtacle()
{
    PrimaryActorTick.bCanEverTick = false;

    DeathCollision = CreateDefaultSubobject<UBoxComponent>("DeathCollision");
    DeathCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
    DeathCollision->SetCollisionResponseToChannel(AsteroidCollisionChannel, ECollisionResponse::ECR_Overlap);

    SetRootComponent(DeathCollision);
}

void AWorldObtacle::BeginPlay()
{
    Super::BeginPlay();

    checkf(DeathCollision, TEXT("DeathCollision doesn't exist!"));

    DeathCollision->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnDeathCollisionEndOverlap);
}

void AWorldObtacle::OnDeathCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent,  //
    AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)             //
{
    if (!OtherActor || !GetWorld()) return;

    GetWorld()->DestroyActor(OtherActor);
}