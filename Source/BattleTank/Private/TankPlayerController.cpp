#include "TankPlayerController.h"
#include "Tank.h"
#include "TankAimingComponent.h"
#include "Engine/World.h"


void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();
	auto possessedTank = Cast<ATank>(GetPawn());
	if (!ensure(possessedTank)) return;
	possessedTank->SetMaxHealth(MaxHealth);
	auto aimingComponent = possessedTank->FindComponentByClass<UTankAimingComponent>();
	if (!ensure(aimingComponent)) return;
	aimingComponent->SetTimeToReload(TimeToReloadInSeconds);
	FoundAimingComponent(aimingComponent);
	aimingComponent->SetMaxAmmo(MaxAmmo);
	EndedSetup();
}

void ATankPlayerController::Tick(float Deltaseconds) {
	Super::Tick(Deltaseconds);
	AimAtCrosshair();
}

void ATankPlayerController::AimAtCrosshair() {

	FVector HitLocation;
	auto ControlledTank = GetPawn();
	if (!ControlledTank) return;
	auto AimingComponent = ControlledTank->FindComponentByClass<UTankAimingComponent>();
	if (!ensure(AimingComponent))
		return; 
	if (GetCrosshairLocation(HitLocation)) {
		AimingComponent->AimAt(HitLocation);
	}

}

bool ATankPlayerController::GetCrosshairLocation(FVector & OutHitLocation) const
{
	int32 ViewportSizeX, ViewportSizeY;
	GetViewportSize(ViewportSizeX, ViewportSizeY);
	FVector ViewportWorldLocation, ViewportWorldDirection;
	if (DeprojectScreenPositionToWorld(
		ViewportSizeX * CrosshairRelativeLocationX, 
		ViewportSizeY * CrosshairRelativeLocationY, 
		ViewportWorldLocation, 
		ViewportWorldDirection)) {
		FHitResult hit;
		if (GetWorld()->LineTraceSingleByChannel(
			OUT hit, ViewportWorldLocation, ViewportWorldLocation + (ViewportWorldDirection * CrosshairMaxRange),
			ECollisionChannel::ECC_Visibility,
			FCollisionQueryParams(FName(TEXT("")), false, GetOwner())
		)) {
			OutHitLocation = hit.Location;
			return true;
		}
	}
	OutHitLocation = FVector(0);
	return false;
}

void ATankPlayerController::SetPawn(APawn * InPawn)
{
	Super::SetPawn(InPawn);
	if (!InPawn) return;
	auto possessedTank = Cast<ATank>(InPawn);
	if (!ensure(possessedTank)) return;
	possessedTank->OnDeathDelegate.AddUniqueDynamic(this, &ATankPlayerController::OnTankDeath);

}

void ATankPlayerController::OnTankDeath()
{
	StartSpectatingOnly();
}
