// Fill out your copyright notice in the Description page of Project Settings.

#include "TankBarrel.h"
#include "Engine/World.h"

void UTankBarrel::Elevate(float RelativeSpeed) {
	
	RelativeSpeed = FMath::Clamp(RelativeSpeed, -1.f, 1.f);

	auto elevationDegrees = RelativeSpeed * MaxDegreesPerSecond * GetWorld()->DeltaTimeSeconds;
	auto nextElevationChange = RelativeRotation.Pitch + elevationDegrees;

	nextElevationChange = FMath::Clamp(nextElevationChange, MinElevationDegree, MaxElevationDegree);
	SetRelativeRotation(FRotator(nextElevationChange, 0, 0));
}



