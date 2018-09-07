// Fill out your copyright notice in the Description page of Project Settings.

#include "MachineTurret.h"
#include "Engine/World.h"

void UMachineTurret::Rotate(float RelativeSpeed)
{
	RelativeSpeed = FMath::Clamp(RelativeSpeed, -1.f, 1.f);
	auto RotationDegrees = RelativeSpeed * MaxDegreesPerSecond * GetWorld()->DeltaTimeSeconds;
	auto nextRotationDegree = RelativeRotation.Yaw + RotationDegrees;

	SetRelativeRotation(FRotator(0, nextRotationDegree, 0));
}
