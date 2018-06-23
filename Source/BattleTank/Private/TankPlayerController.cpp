// Fill out your copyright notice in the Description page of Project Settings.
#include "Engine/World.h"
#include "TankPlayerController.h"

ATank* ATankPlayerController::GetControlledTank() const {
	return Cast<ATank>(GetPawn());
}

void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();

	auto controlledTank = GetControlledTank();
	if (!controlledTank) {
		UE_LOG(LogTemp, Error, TEXT("Player not possessing any tank!"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Player posseessing %s tank"), *controlledTank->GetName());
	}
}

void ATankPlayerController::Tick(float Deltaseconds) {
	if (!GetControlledTank()) return;
	AimAtCrosshair();
}

void ATankPlayerController::AimAtCrosshair() {

	FVector HitLocation;
	if (GetCrosshairLocation(HitLocation)) {
		UE_LOG(LogTemp, Warning, TEXT("Crosshair hit location: %s"), *HitLocation.ToString());
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Can't get crosshair hit location!"));
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
