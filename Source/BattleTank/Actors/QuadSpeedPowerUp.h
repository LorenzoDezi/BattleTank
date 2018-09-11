// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/PowerUp.h"
#include "QuadSpeedPowerUp.generated.h"

/**
 * 
 */
UCLASS()
class BATTLETANK_API AQuadSpeedPowerUp : public APowerUp
{
	GENERATED_BODY()
	
	virtual void PowerUp(AMachine* machine) override;
	
	
};
