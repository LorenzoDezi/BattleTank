// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "TankBarrel.generated.h"

/**
 * 
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class BATTLETANK_API UTankBarrel : public UStaticMeshComponent
{
	GENERATED_BODY()
	
public:
	void Elevate(float RelativeSpeed);
private:
	UPROPERTY(Category = Setup, EditAnywhere)
	float MaxDegreesPerSecond = 20.f; //Sensible value
	UPROPERTY(Category = Setup, EditAnywhere)
	float MaxElevationDegree = 0.f;
	UPROPERTY(Category = Setup, EditAnywhere)
	float MinElevationDegree = 0.f;
};