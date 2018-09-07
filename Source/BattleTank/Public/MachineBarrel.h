// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "MachineBarrel.generated.h"

/**
 * 
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class BATTLETANK_API UMachineBarrel : public UStaticMeshComponent
{
	GENERATED_BODY()
	
public:
	void Elevate(float RelativeSpeed);
private:
	UPROPERTY(Category = Setup, EditDefaultsOnly)
	float MaxDegreesPerSecond = 20.f; //Sensible value
	UPROPERTY(Category = Setup, EditDefaultsOnly)
	float MaxElevationDegree = 0.f;
	UPROPERTY(Category = Setup, EditDefaultsOnly)
	float MinElevationDegree = 0.f;
};
