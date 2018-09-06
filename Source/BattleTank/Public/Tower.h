// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tower.generated.h"

class ATank;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAlarmDelegate, ATower*, Tower);

UCLASS()
class BATTLETANK_API ATower : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATower();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called when a tank spawned by this tower gets killed
	void OnTankDeath();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	FAlarmDelegate OnAlarmDelegate;
	UFUNCTION(BlueprintPure)
	float GetHealthPercent() const;
	virtual float TakeDamage
	(
		float DamageAmount,
		struct FDamageEvent const & DamageEvent,
		class AController * EventInstigator,
		AActor * DamageCauser
	) override;

private:
	//Spawn a tank at the mesh socket location
	ATank* SpawnTank();
	UStaticMeshComponent * Mesh = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
	TSubclassOf<ATank> TankToSpawn = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
	float TimeToSpawn = 5.f;
	float LastTimeATankSpawned = 0.f;
	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
	int32 MaxTanks = 5;
	int32 NumberOfTanksSpawned = 0;
	int32 MaxHealth = 100;
	UPROPERTY(VisibleAnywhere, Category = "Health")
	int32 Health = MaxHealth;
	bool HasSpawnedEmergencyTank = false;
};
