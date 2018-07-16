
#include "Tank.h"
#include "Projectile.h"
#include "Engine/World.h"
#include "Engine/StaticMeshSocket.h"
#include "TankBarrel.h"
#include "TankAimingComponent.h"



// Sets default values
ATank::ATank()
{
	PrimaryActorTick.bCanEverTick = false;
}

float ATank::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	if (Health - DamageAmount < 0) {
		Health = 0;
		return 0;
	}
	else {
		Health -= DamageAmount;
		return DamageAmount;
	}

}
