#include "TankPlayerController.h"
#include "Tank.h"
#include "Engine/World.h"


void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void ATankPlayerController::Tick(float Deltaseconds) {
	Super::Tick(Deltaseconds);
	AimAtCrosshair();
}

ATank* ATankPlayerController::GetControlledTank() {
	return Cast<ATank>(GetPawn());
}

void ATankPlayerController::AimAtCrosshair() {

	FVector HitLocation;
	auto ControlledTank = Cast<ATank>(GetPawn());
	if (GetCrosshairLocation(HitLocation)) {
		ControlledTank -> AimAt(HitLocation);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Can't get crosshair hit location!"));
		Cast<ATank>(GetPawn())->AimAt(ControlledTank->GetActorForwardVector());
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
