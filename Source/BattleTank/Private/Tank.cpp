
#include "Tank.h"
#include "Projectile.h"
#include "Engine/World.h"
#include "Engine/StaticMeshSocket.h"
#include "TankBarrel.h"
#include "TankAimingComponent.h"
#include "TankMovementComponent.h"



// Sets default values
ATank::ATank()
{
	PrimaryActorTick.bCanEverTick = false;
	AimingComponent = CreateDefaultSubobject<UTankAimingComponent>(FName("Aiming_Component"));
	MovementComponent = CreateDefaultSubobject<UTankMovementComponent>(FName("Movement_Component"));
}

void ATank::SetBarrel(UTankBarrel * BarrelToSet)
{
	Barrel = BarrelToSet;
	AimingComponent->SetBarrel(BarrelToSet);
}

void ATank::SetTurret(UTankTurret * TurretToSet)
{
	AimingComponent->SetTurret(TurretToSet);
}

void ATank::Fire()
{
	if (!Barrel) return;

	bool isReloaded = (FPlatformTime::Seconds() - LastFireTime) > TimeToReloadInSeconds;
	const UStaticMeshSocket * projectileSocket = Barrel->GetSocketByName("Projectile");
	FTransform transform;
	if (projectileSocket->GetSocketTransform(transform, Barrel) && isReloaded && Projectile) {
		AProjectile* projectile = GetWorld()-> SpawnActor<AProjectile>(Projectile,transform);
		projectile->Launch(LaunchSpeed);
		LastFireTime = FPlatformTime::Seconds();
	}
}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATank::AimAt(FVector AimLocation)
{
	AimingComponent->AimAt(AimLocation, LaunchSpeed);
}

