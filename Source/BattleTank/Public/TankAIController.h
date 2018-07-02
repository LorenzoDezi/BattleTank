// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TankAIController.generated.h"

class ATank;

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
private:
	void AimAtPlayer();

	UPROPERTY(EditDefaultsOnly, Category = Movement)
	float AcceptanceRadius = 30.f;
		
};
