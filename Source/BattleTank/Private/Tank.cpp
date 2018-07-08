
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
