
#include "Tank.h"
#include "TankAimingComponent.h"



// Sets default values
ATank::ATank()
{
	PrimaryActorTick.bCanEverTick = false;
	AimingComponent = CreateDefaultSubobject<UTankAimingComponent>(FName("Aiming_Component"));
}

void ATank::SetBarrel(UTankBarrel * BarrelToSet)
{
	AimingComponent->SetBarrel(BarrelToSet);
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

