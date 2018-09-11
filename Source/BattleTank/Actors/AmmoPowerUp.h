// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/PowerUp.h"
#include "AmmoPowerUp.generated.h"

/**
 * 
 */
UCLASS()
class BATTLETANK_API AAmmoPowerUp : public APowerUp
{
	GENERATED_BODY()
	
	virtual void PowerUp(AMachine* machine) override;
	
};
