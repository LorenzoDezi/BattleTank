// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/MachineAimingComponent.h"
#include "Components/MachineBarrel.h"
#include "Components/MachineTurret.h"
#include "Actors/Projectile.h"
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
	if (IsQuadSpeed) {
		CheckAim(TimeToReloadInSeconds / 4.f);
		if (GetWorld()->GetTimeSeconds() >= LastTimeQuadSpeed + MaxTimeQuadSpeed) {
			IsQuadSpeed = false;
		}
	}
	else {
		CheckAim(TimeToReloadInSeconds);
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

void UMachineAimingComponent::RecoverAmmo(int32 AmmoRegen)
{
	if (MaxAmmo - CurrentAmmo < AmmoRegen)
		CurrentAmmo += AmmoRegen;
	else
		CurrentAmmo = MaxAmmo;
}

void UMachineAimingComponent::StartQuadSpeed(float MaxTimeQuadSpeed)
{
	IsQuadSpeed = true;
	LastTimeQuadSpeed = GetWorld()->GetTimeSeconds();
	this->MaxTimeQuadSpeed = MaxTimeQuadSpeed;
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
	float direction = FVector::DotProduct(Turret->GetUpVector(),FVector::CrossProduct(Turret->GetForwardVector(), Direction));
	Turret->Rotate(direction);
	Barrel->Elevate(DeltaRotator_1.Pitch);
}

void UMachineAimingComponent::CheckAim(float TimeToReloadInSeconds)
{
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

void UMachineAimingComponent::AimSuspiciously()
{
	Turret->RotateSuspiciously();
}

void UMachineAimingComponent::RotateTurretTowards(FVector Direction)
{
	Turret->RotateTowards(Direction);
}


