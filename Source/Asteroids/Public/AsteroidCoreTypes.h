#pragma once

const FName AsteroidCollisionProfileName = "Asteroid";
const FName ProjectileCollisionProfileName = "Projectile";

#define AsteroidCollisionChannel ECollisionChannel::ECC_GameTraceChannel1

#define TraceChannel_Weapon ECollisionChannel::ECC_GameTraceChannel2