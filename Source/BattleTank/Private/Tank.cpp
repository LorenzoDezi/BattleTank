
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
	AimingComponent = CreateDefaultSubobject<UTankAimingComponent>(FName("Aiming_Component"));
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
	//TODO
	UE_LOG(LogTemp, Warning, TEXT("FIRE!"));
	//Si poteva fare con getSocketLocation e getSocketRotation ... 
	const UStaticMeshSocket * projectileSocket = Barrel->GetSocketByName("Projectile");
	FTransform transform;
	if (projectileSocket->GetSocketTransform(transform, Barrel)) {
		AProjectile* projectile = GetWorld()-> SpawnActor<AProjectile>(Projectile,transform);
		projectile->Launch(LaunchSpeed);
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

