// A test task by KEFIR
#include "Weapons/Ranged/Laser.h"

ALaser::ALaser() {}

void ALaser::BeginPlay()
{
    Super::BeginPlay();
    checkf(ShotRange > 0, TEXT("ShotRange must be more than zero!"));
}

void ALaser::StartFire()
{
    Super::StartFire();
}

void ALaser::StopFire()
{
    Super::StopFire();
}
