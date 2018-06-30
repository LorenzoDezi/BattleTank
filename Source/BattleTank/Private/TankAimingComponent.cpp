// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAimingComponent.h"
#include "TankBarrel.h"
#include "TankTurret.h"
#include "Engine/World.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	//TODO: The aiming component should tick? Or not?
	PrimaryComponentTick.bCanEverTick = true;
}


void UTankAimingComponent::SetBarrel(UTankBarrel * BarrelToSet)
{
	Barrel = BarrelToSet;
}

void UTankAimingComponent::SetTurret(UTankTurret * TurretToSet)
{
	Turret = TurretToSet;
}

// Called when the game starts
void UTankAimingComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UTankAimingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UTankAimingComponent::AimAt(FVector AimLocation, float LaunchSpeed)
{
	if (!Barrel) return;
	FVector StartLocation = Barrel->GetSocketLocation(FName("Projectile"));
	FVector OutVelocity;
	float Time = GetWorld()->GetTimeSeconds();
	if (UGameplayStatics::SuggestProjectileVelocity(this, OutVelocity, StartLocation, 
		AimLocation, LaunchSpeed, false, 0.f, 0.f, ESuggestProjVelocityTraceOption::DoNotTrace)) {
		FVector AimDirection = OutVelocity.GetSafeNormal();
		MoveBarrelTowards(AimDirection);
		UE_LOG(LogTemp, Warning, TEXT("%f: %s component of tank %s aiming at %s, direction %s"),
			Time,
			*GetName(),
			*(GetOwner()->GetName()),
			*AimLocation.ToString(),
			*AimDirection.ToString());
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("%f: %s didn't found aim solution."),
			Time,
			*GetName());

	}
	
}

void UTankAimingComponent::MoveBarrelTowards(FVector Direction)
{
	if (!Barrel) return;
	if (!Turret) return;

	FRotator BarrelRotator = Barrel->GetForwardVector().Rotation();
	FRotator AimAsRotator = Direction.Rotation();
	auto DeltaRotator = AimAsRotator - BarrelRotator;

	Barrel->Elevate(DeltaRotator.Pitch);
	Turret->Rotate(DeltaRotator.Yaw);
}

