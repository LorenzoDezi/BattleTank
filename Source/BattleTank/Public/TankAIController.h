// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TankAIController.generated.h"

/**
 * 
 */
UCLASS()
class BATTLETANK_API ATankAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;	
	virtual void Tick(float Deltaseconds) override;
	UFUNCTION(BlueprintImplementableEvent, Category = "Setup")
	void EndedSetup();
private:
	void AimAtPlayer();
	UFUNCTION()
	void OnTankDeath();
	virtual void SetPawn(APawn* InPawn) override;
	UPROPERTY(EditDefaultsOnly, Category = Movement)
	float AcceptanceRadius = 100.f;
	UPROPERTY(EditDefaultsOnly, Category = "Game Variables")
	int32 MaxHealth = 100.f;
	UPROPERTY(EditDefaultsOnly, Category = "Game Variables")
	float TimeToReloadInSeconds = 2.f;
};
