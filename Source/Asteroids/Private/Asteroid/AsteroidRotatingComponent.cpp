// A test task by KEFIR
#include "Asteroid/AsteroidRotatingComponent.h"
#include "Components/SkeletalMeshComponent.h"

void UAsteroidRotatingComponent::InitializeComponent()
{
    if (bAutoRegisterUpdatedComponent)
    {
        if (AActor* MyOwner = GetOwner())
        {
            if (USceneComponent* NewUpdatedComponent = MyOwner->FindComponentByClass<USkeletalMeshComponent>())
            {
                SetUpdatedComponent(NewUpdatedComponent);
            }
        }
    }
    Super::InitializeComponent();
}

void UAsteroidRotatingComponent::OnRegister()
{
    if (AActor* MyOwner = GetOwner())
    {
        if (USceneComponent* NewUpdatedComponent = MyOwner->FindComponentByClass<USkeletalMeshComponent>())
        {
            SetUpdatedComponent(NewUpdatedComponent);
        }
    }
    Super::OnRegister();
}

void UAsteroidRotatingComponent::BeginPlay()
{
    Super::BeginPlay();
    {
        TArray<float> CheckingValues{MinRandomPitch, MaxRandomPitch, MinRandomYaw, MaxRandomYaw, MinRandomRoll, MaxRandomRoll};
        for (const float Value : CheckingValues)
        {
            checkf(Value >= 0.0f && Value <= 360.0f, TEXT("Min & Max values for Pitch, Yaw, Roll must be in range [0;360]!"));
        }
    }
    RotationRate = FRotator(FMath::RandRange(MinRandomPitch, MaxRandomPitch), FMath::RandRange(MinRandomYaw, MaxRandomYaw),
        FMath::RandRange(MinRandomRoll, MaxRandomRoll));
}

void UAsteroidRotatingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    if (ShouldSkipUpdate(DeltaTime)) return;
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    if (!IsValid(UpdatedComponent)) return;

    const FQuat OldRotation = UpdatedComponent->GetComponentQuat();
    const FQuat DeltaRotation = (RotationRate * DeltaTime).Quaternion();
    const FQuat NewRotation = OldRotation * DeltaRotation;

    const bool bEnableCollision = false;
    MoveUpdatedComponent(FVector::ZeroVector, NewRotation, bEnableCollision);
}
