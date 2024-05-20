// A test task by KEFIR
#include "Weapons/Ranged/Laser.h"
#include "Kismet/KismetSystemLibrary.h"
#include "AsteroidCoreTypes.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Components/HealthComponent.h"

ALaser::ALaser() {}

void ALaser::BeginPlay()
{
    Super::BeginPlay();
    checkf(ShotRange > 0.0f, TEXT("ShotRange must be more than zero!"));
    checkf(LaserRadius > 0.0f, TEXT("LaserRadius must be more than zero!"));
    checkf(MaxLaserShots > 0, TEXT("MaxLaserShots must be more than zero!"));
    checkf(ReloadingTime > 0.0f, TEXT("ReloadingTime must be more than zero!"));

    SetCurrentLaserShots(MaxLaserShots);
}

void ALaser::StartFire()
{
    MakeShot();
}

void ALaser::StopFire()
{
    if (CurrentLaserShots == 0)
    {
        StartReload();
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
        UEngineTypes::ConvertToTraceType(WeaponTraceCollisionChannel), true, {GetOwner()}, EDrawDebugTrace::Type::None, Hit, true);

    AActor* HitActor = Hit.GetActor();
    if (Hit.bBlockingHit && IsValid(HitActor))
    {
        auto* HitActorHealthComponent = HitActor->FindComponentByClass<UHealthComponent>();
        if (HitActorHealthComponent)
        {
            HitActorHealthComponent->SetHealth(0);
        }
        else
        {
            HitActor->Destroy();
        }
    }

    SetCurrentLaserShots(CurrentLaserShots - 1);

    PlayLaserEffect(Hit);
    PlayShotEffect();
}

void ALaser::PlayLaserEffect(const FHitResult& Hit)
{
    auto* NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), LaserEffect, Hit.TraceStart, GetActorRotation());
    if (!IsValid(NiagaraComponent)) return;

    NiagaraComponent->SetVectorParameter(LaserEndVectorParamaterName, Hit.bBlockingHit ? Hit.ImpactPoint : Hit.TraceEnd);
    NiagaraComponent->SetFloatParameter(LaserWidthFloatParamaterName, LaserRadius * 2.0f);
}

void ALaser::SetCurrentLaserShots(const int32 NewLaserShots)
{
    CurrentLaserShots = FMath::Clamp(NewLaserShots, 0, MaxLaserShots);
    OnLaserShotsChanged.Broadcast(CurrentLaserShots);
}

void ALaser::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
    const FTransform& MuzzleSocketTransform = GetMuzzleSocketTransform();

    const FVector ShotDirection = MuzzleSocketTransform.GetRotation().Vector();
    TraceStart = MuzzleSocketTransform.GetLocation();
    TraceEnd = TraceStart + ShotDirection * ShotRange;
}

void ALaser::StartReload()
{
    UWorld* World = GetWorld();
    if (bReloading || World == nullptr) return;

    OnReloadingStarted.Broadcast(ReloadingTime);
    bReloading = true;

    World->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ThisClass::StopReload, ReloadingTime, false, -1.0f);
}
void ALaser::StopReload()
{
    OnReloadingFinished.Broadcast();
    bReloading = false;

    SetCurrentLaserShots(MaxLaserShots);
}

bool ALaser::CanMakeShot() const
{
    return CurrentLaserShots > 0;
}

int32 ALaser::GetCurrentLaserShots() const
{
    return CurrentLaserShots;
}

float ALaser::GetReloadingTime() const
{
    return ReloadingTime;
}
