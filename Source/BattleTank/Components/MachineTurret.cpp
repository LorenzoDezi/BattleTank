// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/MachineTurret.h"
#include "Engine/World.h"

void UMachineTurret::Rotate(float RelativeSpeed)
{
	UE_LOG(LogTemp, Warning, TEXT("TURRET Relative Speed: %f %s"), RelativeSpeed, *GetAttachmentRootActor()->GetName());
	RelativeSpeed = FMath::Clamp(RelativeSpeed, -1.f, 1.f);
	auto RotationDegrees = RelativeSpeed * MaxDegreesPerSecond * GetWorld()->DeltaTimeSeconds;
	auto nextRotationDegree = RelativeRotation.Yaw + RotationDegrees;

	SetRelativeRotation(FRotator(0, nextRotationDegree, 0));
}

void UMachineTurret::RotateSuspiciously()
{
	auto RotationDegrees =  MaxDegreesPerSecond * GetWorld()->DeltaTimeSeconds;
	float nextRotationDegree = 0.f;
	if (IsSuspiciouslyRotatingToNinetyDeg) {
		nextRotationDegree = RelativeRotation.Yaw + RotationDegrees;
		if (nextRotationDegree >= 90.f) IsSuspiciouslyRotatingToNinetyDeg = false;
	}
	else {
		nextRotationDegree = RelativeRotation.Yaw - RotationDegrees;
		if (nextRotationDegree <= -90.f) IsSuspiciouslyRotatingToNinetyDeg = true;
	}
	SetRelativeRotation(FRotator(0, nextRotationDegree, 0));
}

void UMachineTurret::RotateTowards(FVector Direction) {
	float RelativeSpeed = FVector::CrossProduct(GetForwardVector(), Direction).Size();
	Rotate(RelativeSpeed);
}
