// Fill out your copyright notice in the Description page of Project Settings.

#include "Tower.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "Tank.h"


// Sets default values
ATower::ATower()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATower::BeginPlay()
{
	Super::BeginPlay();
	Health = MaxHealth;
	Mesh = FindComponentByClass<UStaticMeshComponent>();
}

void ATower::OnTankDeath()
{
	NumberOfTanksSpawned--;
}

void ATower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!Mesh || !TankToSpawn || NumberOfTanksSpawned > MaxTanks || GetWorld()->GetTimeSeconds() + TimeToSpawn > LastTimeATankSpawned) return;
	auto tankSpawned = SpawnTank();
	tankSpawned->OnDeathDelegate.AddDynamic(this, &ATower::OnTankDeath);
	NumberOfTanksSpawned++;
	LastTimeATankSpawned = GetWorld()->GetTimeSeconds();
}

float ATower::GetHealthPercent() const
{
	return (float)Health / (float)MaxHealth;
}

float ATower::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	//Broadcast the alarm to the tanks
	OnAlarmDelegate.Broadcast(this);
	//The tower is already dead
	if (!Health) {
		return 0.f;
	}
	//Apply damage
	float DamageApplied = 0.f;
	if (Health - DamageAmount < 0) {
		DamageApplied = Health;
		Health = 0;
	}
	else {
		Health -= DamageAmount;
		DamageApplied = DamageAmount;
		if (Health <= MaxHealth / 4 && !HasSpawnedEmergencyTank) {
			SpawnTank();
			HasSpawnedEmergencyTank = true;
		}
	}
	
	return DamageApplied;
}

ATank* ATower::SpawnTank()
{
	FVector Location = Mesh->GetSocketLocation(FName("TankSpawn"));
	FRotator Rotation = Mesh->GetSocketRotation(FName("TankSpawn"));
	ATank* tankSpawned = GetWorld()->SpawnActor<ATank>(TankToSpawn, Location, Rotation);
	OnAlarmDelegate.AddDynamic(tankSpawned, &ATank::OnMotherTowerAlarm);
	return tankSpawned;
}

