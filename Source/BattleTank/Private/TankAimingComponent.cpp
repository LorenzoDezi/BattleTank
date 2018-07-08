// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAimingComponent.h"
#include "TankBarrel.h"
#include "TankTurret.h"
#include "Projectile.h"
#include "Engine/StaticMeshSocket.h"
#include "Engine/World.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	//TODO: The aiming component should tick? Or not?
	PrimaryComponentTick.bCanEverTick = false;
}

void UTankAimingComponent::Initialise(UTankBarrel * BarrelToSet, UTankTurret * TurretToSet)
{
	Barrel = BarrelToSet;
	Turret = TurretToSet;
}

// Called when the game starts
void UTankAimingComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UTankAimingComponent::AimAt(FVector AimLocation)
{
	if (!ensure(Barrel)) return;
	FVector StartLocation = Barrel->GetSocketLocation(FName("Projectile"));
	FVector OutVelocity;
	float Time = GetWorld()->GetTimeSeconds();
	if (UGameplayStatics::SuggestProjectileVelocity(this, OutVelocity, StartLocation, 
		AimLocation, LaunchSpeed, false, 0.f, 0.f, ESuggestProjVelocityTraceOption::DoNotTrace)) {
		FVector AimDirection = OutVelocity.GetSafeNormal();
		MoveBarrelTowards(AimDirection);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("%f: %s didn't found aim solution."),
			Time,
			*GetName());
	}	
}

void UTankAimingComponent::Fire()
{
	if (!ensure(Barrel)) return;

	bool isReloaded = (FPlatformTime::Seconds() - LastFireTime) > TimeToReloadInSeconds;
	const UStaticMeshSocket * projectileSocket = Barrel->GetSocketByName("Projectile");
	FTransform transform;
	if (projectileSocket->GetSocketTransform(transform, Barrel) && isReloaded && Projectile) {
		AProjectile* projectile = GetWorld()->SpawnActor<AProjectile>(Projectile, transform);
		projectile->Launch(LaunchSpeed);
		LastFireTime = FPlatformTime::Seconds();
	}
}

void UTankAimingComponent::MoveBarrelTowards(FVector Direction)
{
	if (!ensure(Barrel)) return;
	if (!ensure(Turret)) return;

	FRotator BarrelRotator = Barrel->GetForwardVector().Rotation();
	FRotator AimAsRotator = Direction.Rotation();
	auto DeltaRotator = AimAsRotator - BarrelRotator;

	Barrel->Elevate(DeltaRotator.Pitch);
	Turret->Rotate(DeltaRotator.Yaw);
}

