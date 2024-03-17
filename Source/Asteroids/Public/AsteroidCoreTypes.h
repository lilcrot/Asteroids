#pragma once

const FName AsteroidCollisionProfileName = "Asteroid";
const FName ProjectileCollisionProfileName = "Projectile";
const FName WorldBoundaryCollisionProfileName = "WorldBoundary";
const FName PlayerCollisionProfileName = "Player";

#define WeaponTraceCollisionChannel ECollisionChannel::ECC_GameTraceChannel2
#define ProjectileCollisionChannel ECollisionChannel::ECC_GameTraceChannel3