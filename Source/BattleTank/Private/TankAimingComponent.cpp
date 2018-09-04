// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAimingComponent.h"
#include "TankBarrel.h"
#include "TankTurret.h"
#include "Projectile.h"
#include "Engine/StaticMeshSocket.h"
#include "Engine/World.h"
#include "Components/AudioComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
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
	CurrentAmmo = MaxAmmo;
	if (CurrentAmmo == 0)
		FiringState = EFiringState::OutOfAmmo;
}

void UTankAimingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	if (!Barrel || FiringState == EFiringState::OutOfAmmo) return;

	if (GetWorld()->GetTimeSeconds() < LastFireTime + TimeToReloadInSeconds)
		FiringState = EFiringState::Reloading;
	else if (!(Barrel->GetForwardVector().GetSafeNormal().Equals(AimDirection, 0.1f)))
		FiringState = EFiringState::Aiming;
	else
		FiringState = EFiringState::Locked;
}

void UTankAimingComponent::AimAt(FVector AimLocation)
{
	if (!Barrel) return;
	FVector StartLocation = Barrel->GetSocketLocation(FName("Projectile"));
	FVector OutVelocity;
	if (UGameplayStatics::SuggestProjectileVelocity(this, OutVelocity, StartLocation, 
		AimLocation, LaunchSpeed, false, 0.f, 0.f, ESuggestProjVelocityTraceOption::DoNotTrace)) {
		AimDirection = OutVelocity.GetSafeNormal();
		MoveBarrelTowards(AimDirection);
	}
	
}

void UTankAimingComponent::Fire()
{
	UE_LOG(LogTemp, Warning, TEXT("Fire called %s"), *Barrel->GetName());
	if (!Barrel) return;
	const UStaticMeshSocket * projectileSocket = Barrel->GetSocketByName("Projectile");
	UE_LOG(LogTemp, Warning, TEXT("Fire called %s"), *projectileSocket->GetName());

	if (!projectileSocket || !Projectile) return;
	if (FiringState == EFiringState::OutOfAmmo) {
		if(TankOutOfAmmo)
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), TankOutOfAmmo,
				Barrel->GetComponentLocation());
		UE_LOG(LogTemp, Warning, TEXT("Tank Out Of Ammo"));
		return;

	}

	FTransform transform;
	if (projectileSocket->GetSocketTransform(transform, Barrel) 
		&& FiringState != EFiringState::Reloading ) {
		UE_LOG(LogTemp, Warning, TEXT("Shooting"));
		AProjectile* projectile = GetWorld()->SpawnActor<AProjectile>(Projectile, transform);
		projectile->Launch(LaunchSpeed);
		if(TankFire)
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), TankFire,
				Barrel->GetComponentLocation());
		FiringState = EFiringState::Reloading;
		LastFireTime = GetWorld()->GetTimeSeconds();
		if (CurrentAmmo > 0)
			CurrentAmmo--;
		if(CurrentAmmo == 0)
			FiringState = EFiringState::OutOfAmmo;
		//The -1 default value stands for infinite ammo
		if (CurrentAmmo == -1)
			CurrentAmmo = 1;
		OnFireDelegate.Broadcast();
	}
}

const int32 UTankAimingComponent::GetCurrentAmmo()
{
	UE_LOG(LogTemp, Warning, TEXT("CURRENT AMMO - %d"), CurrentAmmo);
	return CurrentAmmo;
}

void UTankAimingComponent::SetMaxAmmo(int32 MaxAmmo)
{
	this->MaxAmmo = MaxAmmo;
	CurrentAmmo = MaxAmmo;
}

void UTankAimingComponent::SetTimeToReload(float timeToReloadInSeconds)
{
	TimeToReloadInSeconds = timeToReloadInSeconds;
}

const EFiringState UTankAimingComponent::GetFiringState()
{
	return FiringState;
}

void UTankAimingComponent::MoveBarrelTowards(FVector Direction)
{
	if (!ensure(Barrel)) return;
	if (!ensure(Turret)) return;
	FRotator BarrelRotator = Barrel->GetForwardVector().Rotation();
	FRotator AimAsRotator = Direction.Rotation();

	auto DeltaRotator_1 = AimAsRotator - BarrelRotator;
	if (FMath::Abs(DeltaRotator_1.Yaw) > 180) {
		Turret->Rotate(-DeltaRotator_1.Yaw);
	}
	else {
		Turret->Rotate(DeltaRotator_1.Yaw);

	}

	Barrel->Elevate(DeltaRotator_1.Pitch);
}

