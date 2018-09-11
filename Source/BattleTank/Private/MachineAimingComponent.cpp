// Fill out your copyright notice in the Description page of Project Settings.

#include "MachineAimingComponent.h"
#include "MachineBarrel.h"
#include "MachineTurret.h"
#include "Projectile.h"
#include "Engine/StaticMeshSocket.h"
#include "Engine/World.h"
#include "Components/AudioComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UMachineAimingComponent::UMachineAimingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UMachineAimingComponent::Initialise(UMachineBarrel * BarrelToSet, UMachineTurret * TurretToSet)
{
	Barrel = BarrelToSet;
	Turret = TurretToSet;
}

// Called when the game starts
void UMachineAimingComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentAmmo = MaxAmmo;
	if (CurrentAmmo == 0)
		FiringState = EFiringState::OutOfAmmo;
}

void UMachineAimingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	if (!Barrel || FiringState == EFiringState::OutOfAmmo) return;

	if (GetWorld()->GetTimeSeconds() < LastFireTime + TimeToReloadInSeconds) {
		FiringState = EFiringState::Reloading;
	}
	else if (!(Barrel->GetForwardVector().GetSafeNormal().Equals(AimDirection, 0.2f))) {
		FiringState = EFiringState::Aiming;
	}
	else {
		FiringState = EFiringState::Locked;
	}
}

void UMachineAimingComponent::AimAt(FVector AimLocation)
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

void UMachineAimingComponent::Fire()
{
	if (!Barrel) return;
	const UStaticMeshSocket * projectileSocket = Barrel->GetSocketByName("Projectile");
	if (!projectileSocket || !Projectile) return;
	if (FiringState == EFiringState::OutOfAmmo) {
		if(TankOutOfAmmo)
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), TankOutOfAmmo,
				Barrel->GetComponentLocation());
		return;
	}

	FTransform transform;
	if (projectileSocket->GetSocketTransform(transform, Barrel) 
		&& FiringState != EFiringState::Reloading ) {
		AProjectile* projectile = GetWorld()->SpawnActor<AProjectile>(Projectile, transform);
		projectile->SetTankWhoShot(GetOwner());
		projectile->Launch(LaunchSpeed);
		if(TankFire)
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), TankFire,
				Barrel->GetComponentLocation());
		LastFireTime = GetWorld()->GetTimeSeconds();
		FiringState = EFiringState::Reloading;
		if (CurrentAmmo > 0)
			CurrentAmmo--;
		//The -1 default value stands for infinite ammo
		if (MaxAmmo == -1)
			CurrentAmmo = 1;
		if (CurrentAmmo == 0)
			FiringState = EFiringState::OutOfAmmo;
		OnFireDelegate.Broadcast();
	}
}

const int32 UMachineAimingComponent::GetCurrentAmmo()
{
	return CurrentAmmo;
}

void UMachineAimingComponent::SetMaxAmmo(int32 MaxAmmo)
{
	this->MaxAmmo = MaxAmmo;
	CurrentAmmo = MaxAmmo;
}

void UMachineAimingComponent::SetTimeToReload(float timeToReloadInSeconds)
{
	TimeToReloadInSeconds = timeToReloadInSeconds;
}

const EFiringState UMachineAimingComponent::GetFiringState()
{
	return FiringState;
}

void UMachineAimingComponent::MoveBarrelTowards(FVector Direction)
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

