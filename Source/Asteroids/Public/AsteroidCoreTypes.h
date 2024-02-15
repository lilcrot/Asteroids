#pragma once

const FName AsteroidCollisionProfileName = "Asteroid";
const FName ProjectileCollisionProfileName = "Projectile";

#define AsteroidCollisionChannel ECollisionChannel::ECC_GameTraceChannel1
#define WeaponTraceCollisionChannel ECollisionChannel::ECC_GameTraceChannel2
#define ProjectileCollisionChannel ECollisionChannel::ECC_GameTraceChannel3