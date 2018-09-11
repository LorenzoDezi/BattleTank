// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/PowerUp.h"
#include "HealthPowerUp.generated.h"

/**
 * 
 */
UCLASS()
class BATTLETANK_API AHealthPowerUp : public APowerUp
{
	GENERATED_BODY()
	
public:
	virtual void PowerUp(AMachine* machine) override;
};
