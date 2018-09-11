// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WidgetAssociatedActor.h"
#include "Tower.generated.h"

class AMachine;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAlarmDelegate, AActor*, Attacker);

UCLASS()
class BATTLETANK_API ATower : public AActor, public IWidgetAssociatedActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATower();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION()
	// Called when a tank spawned by this tower gets killed
	void OnTankDeath();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	FAlarmDelegate OnAlarmDelegate;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Health")
	float GetHealthPercent() const;
	virtual float GetHealthPercent_Implementation() const;
	virtual float TakeDamage
	(
		float DamageAmount,
		struct FDamageEvent const & DamageEvent,
		class AController * EventInstigator,
		AActor * DamageCauser
	) override;

private:
	//Spawn a tank at the mesh socket location
	AMachine* SpawnTank();

	UStaticMeshComponent * Mesh = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
	TSubclassOf<AMachine> TankToSpawn = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
	float TimeToSpawn = 5.f;
	float LastTimeATankSpawned = 0.f;
	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
	int32 MaxTanks = 5;
	int32 NumberOfTanksSpawned = 0;
	UPROPERTY(EditDefaultsOnly, Category = "Health")
	int32 MaxHealth = 100;
	UPROPERTY(VisibleAnywhere, Category = "Health")
	int32 Health = MaxHealth;
	bool HasSpawnedEmergencyTank = false;
};
