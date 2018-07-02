// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h" 

class ATank;

UCLASS()
class BATTLETANK_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	
	void BeginPlay() override;
	virtual void Tick(float Deltaseconds) override;


private:
	UFUNCTION(BlueprintPure, Category = Setup)
	ATank * GetControlledTank();
	void AimAtCrosshair();
	bool GetCrosshairLocation(FVector& OutHitLocation) const;

	UPROPERTY(EditDefaultsOnly)
	float CrosshairRelativeLocationX = 0.5f;
	UPROPERTY(EditDefaultsOnly)
	float CrosshairRelativeLocationY = 0.33333f;
	UPROPERTY(EditDefaultsOnly)
	float CrosshairMaxRange = 10000.f;
};
