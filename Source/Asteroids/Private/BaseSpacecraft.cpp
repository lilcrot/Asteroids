// A test task by KEFIR
#include "BaseSpacecraft.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"

ABaseSpacecraft::ABaseSpacecraft()
{
    PrimaryActorTick.bCanEverTick = false;

    BoxCollision = CreateDefaultSubobject<UBoxComponent>("BoxCollision");
    SetRootComponent(BoxCollision);

    MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("MeshComponent");
}

void ABaseSpacecraft::BeginPlay()
{
    Super::BeginPlay();
    checkf(BoxCollision, TEXT("BoxCollision doesn't exists!"));
    checkf(MeshComponent, TEXT("MeshComponent doesn't exists!"));
}

void ABaseSpacecraft::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}