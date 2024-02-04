// A test task by KEFIR
#include "BaseSpacecraft.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Components/HealthComponent.h"

ABaseSpacecraft::ABaseSpacecraft()
{
    PrimaryActorTick.bCanEverTick = false;

    BoxCollision = CreateDefaultSubobject<UBoxComponent>("BoxCollision");
    checkf(BoxCollision, TEXT("BoxCollision doesn't exist!"));

    BoxCollision->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
    SetRootComponent(BoxCollision);

    MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("MeshComponent");
    checkf(MeshComponent, TEXT("MeshComponent doesn't exist!"));

    MeshComponent->SetupAttachment(BoxCollision);

    MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>("MovementComponent");
    checkf(MovementComponent, TEXT("MovementComponent doesn't exist!"));

    HealthComponent = CreateDefaultSubobject<UHealthComponent>("HealthComponent");
    checkf(HealthComponent, TEXT("HealthComponent doesn't exist!"));
}

void ABaseSpacecraft::BeginPlay()
{
    Super::BeginPlay();
}

void ABaseSpacecraft::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}