
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
	//The tank is already dead
	if (!Health) {
		return 0.f;
	}
	//Apply damage
	float DamageApplied = 0.f;
	if (Health - DamageAmount < 0) {
		DamageApplied = Health;
		Health = 0;
	}
	else {
		Health -= DamageAmount;
		DamageApplied = DamageAmount;
	}
	//Broadcast death event if dead after the damage
	if (!Health)
		OnDeathDelegate.Broadcast();
	return DamageApplied;
}

float ATank::GetHealthPercent() const
{
	return (float)Health / (float)MaxHealth;
}
