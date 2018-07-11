#include "TankPlayerController.h"
#include "Tank.h"
#include "TankAimingComponent.h"
#include "Engine/World.h"


void ATankPlayerController::BeginPlay()
{
	auto aimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
	if (ensure(aimingComponent))
		FoundAimingComponent(aimingComponent);
	Super::BeginPlay();
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
	else {
		UE_LOG(LogTemp, Error, TEXT("Can't get crosshair hit location!"));
		AimingComponent->AimAt(ControlledTank->GetActorForwardVector());
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
