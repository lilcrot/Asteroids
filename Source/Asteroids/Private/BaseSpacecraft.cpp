// A test task by KEFIR
#include "BaseSpacecraft.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/FloatingPawnMovement.h"

ABaseSpacecraft::ABaseSpacecraft()
{
    PrimaryActorTick.bCanEverTick = false;

    BoxCollision = CreateDefaultSubobject<UBoxComponent>("BoxCollision");
    BoxCollision->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
    SetRootComponent(BoxCollision);

    MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("MeshComponent");
    MeshComponent->SetupAttachment(BoxCollision);

    MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>("MovementComponent");
}

void ABaseSpacecraft::BeginPlay()
{
    Super::BeginPlay();

    {
        checkf(BoxCollision, TEXT("BoxCollision doesn't exist!"));
        checkf(MeshComponent, TEXT("MeshComponent doesn't exist!"));
        checkf(MovementComponent, TEXT("MovementComponent doesn't exist!"));
    }
}

void ABaseSpacecraft::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}