// Fill out your copyright notice in the Description page of Project Settings.

#include "Actors/Tower.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "BattleTankGameModeBase.h"
#include "DestructibleComponent.h"
#include "Engine/World.h"
#include "Actors/Machine.h"


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
	DestructibleMesh = FindComponentByClass<UDestructibleComponent>();	
}

void ATower::OnTankDeath()
{
	NumberOfTanksSpawned--;
}

void ATower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!Health || !DestructibleMesh || !TankToSpawn || NumberOfTanksSpawned > MaxTanks 
		|| GetWorld()->GetTimeSeconds() < LastTimeATankSpawned + TimeToSpawn) 
		return;
	auto tankSpawned = SpawnTank();
	if (!tankSpawned) return;
	tankSpawned->OnDeathDelegate.AddDynamic(this, &ATower::OnTankDeath);
	NumberOfTanksSpawned++;
	LastTimeATankSpawned = GetWorld()->GetTimeSeconds();
}

float ATower::GetHealthPercent_Implementation() const
{
	return (float)Health / (float)MaxHealth;
}

float ATower::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	//Broadcast the alarm to the tanks
	if (!(DamageCauser) || DamageCauser->ActorHasTag(FName("Enemy"))) return 0.f;
	OnAlarmDelegate.Broadcast(DamageCauser);
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
	//Dopo l'ultimo danno, la torre � morta
	if (!Health) {
		TowerDeath(DamageEvent);
	}
	return DamageApplied;
}

void ATower::TowerDeath(const FDamageEvent & DamageEvent)
{
	OnDefeatDelegate.Broadcast();
	auto gameMode = GetWorld()->GetAuthGameMode();
	if (gameMode && gameMode->IsA<ABattleTankGameModeBase>()) {
		Cast<ABattleTankGameModeBase>(gameMode)->TowerDefeated();
	}
	//Distruggo l'attore dopo un tot 
	FTimerHandle Timer;
	GetWorldTimerManager().SetTimer(Timer, this, &ATower::DestroyCall, SecondsToDestroyAfterDeath);
	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID)) {
		FPointDamageEvent* PointDamageEvent = (FPointDamageEvent*)&DamageEvent;
		DestructibleMesh->ApplyDamage(MaxHealth, PointDamageEvent->HitInfo.Location, PointDamageEvent->ShotDirection, 10);

	}
	else if (DamageEvent.IsOfType(FRadialDamageEvent::ClassID)) {
		FRadialDamageEvent* RadialDamageEvent = (FRadialDamageEvent*)&DamageEvent;
		DestructibleMesh->ApplyDamage(MaxHealth, RadialDamageEvent->Origin, RadialDamageEvent->ComponentHits[0].Location, 10);
	}
}

AMachine* ATower::SpawnTank()
{
	TArray<UActorComponent*> spawnPoints = GetComponentsByTag(USceneComponent::StaticClass(), FName("TowerSpawn"));
	if (!spawnPoints.Num()) return nullptr;
	int32 randomIndexChosen = FMath::RandRange(0, spawnPoints.Num() - 1);
	USceneComponent* chosenSpawn = Cast<USceneComponent>(spawnPoints[randomIndexChosen]);
	FVector Location = chosenSpawn->GetComponentLocation();
	FRotator Rotation = chosenSpawn->GetComponentRotation();
	AMachine* tankSpawned = GetWorld()->SpawnActor<AMachine>(TankToSpawn, Location, Rotation);
	if (!tankSpawned) return nullptr;
	OnAlarmDelegate.AddDynamic(tankSpawned, &AMachine::OnMotherTowerAlarm);
	OnDefeatDelegate.AddDynamic(tankSpawned, &AMachine::OnMotherTowerDeath);
	return tankSpawned;
}

void ATower::DestroyCall()
{
	Destroy();
}

