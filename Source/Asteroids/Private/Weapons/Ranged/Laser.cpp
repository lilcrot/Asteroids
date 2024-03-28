// A test task by KEFIR
#include "Weapons/Ranged/Laser.h"
#include "Kismet/KismetSystemLibrary.h"
#include "AsteroidCoreTypes.h"

ALaser::ALaser() {}

void ALaser::BeginPlay()
{
    Super::BeginPlay();
    checkf(ShotRange > 0.0f, TEXT("ShotRange must be more than zero!"));
    checkf(LaserRadius > 0.0f, TEXT("LaserRadius must be more than zero!"));
    checkf(MaxLaserShots > 0, TEXT("MaxLaserShots must be more than zero!"));
    checkf(ReloadingTime > 0.0f, TEXT("ReloadingTime must be more than zero!"));

    CurrentLaserShots = MaxLaserShots;
}

void ALaser::StartFire()
{
    MakeShot();
}

void ALaser::StopFire()
{
    if (CurrentLaserShots == 0)
    {
        StartReloadLaserShots();
    }
}

void ALaser::MakeShot()
{
    if (!CanMakeShot())
    {
        StopFire();
        return;
    }

    FVector TraceStart, TraceEnd;
    GetTraceData(TraceStart, TraceEnd);
    FHitResult Hit;

    UKismetSystemLibrary::SphereTraceSingle(GetWorld(), TraceStart, TraceEnd, LaserRadius,
        UEngineTypes::ConvertToTraceType(WeaponTraceCollisionChannel), true, {GetOwner()}, EDrawDebugTrace::Type::ForDuration, Hit, true);

    AActor* HitActor = Hit.GetActor();
    if (Hit.bBlockingHit && IsValid(HitActor))
    {
        HitActor->Destroy();
    }

    --CurrentLaserShots;
}

bool ALaser::CanMakeShot() const
{
    return CurrentLaserShots > 0;
}

void ALaser::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
    const FTransform& MuzzleSocketTransform = GetMuzzleSocketTransform();

    const FVector ShotDirection = MuzzleSocketTransform.GetRotation().Vector();
    TraceStart = MuzzleSocketTransform.GetLocation();
    TraceEnd = TraceStart + ShotDirection * ShotRange;
}

void ALaser::StartReloadLaserShots()
{
    UWorld* World = GetWorld();
    if (bReloading || World == nullptr) return;

    bReloading = true;
    FTimerHandle ReloadTimerHandle;
    World->GetTimerManager().SetTimer(
        ReloadTimerHandle,
        [&]()
        {
            bReloading = false;
            CurrentLaserShots = MaxLaserShots;
        },
        ReloadingTime, false, -1.0f);
}
