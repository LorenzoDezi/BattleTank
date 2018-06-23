// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Tank.h"
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h" 

/**
 * 
 */
UCLASS()
class BATTLETANK_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	
	ATank * GetControlledTank() const;
	void BeginPlay() override;
	virtual void Tick(float Deltaseconds) override;

private:
	void AimAtCrosshair();
	bool GetCrosshairLocation(FVector& OutHitLocation) const;

	UPROPERTY(EditAnywhere)
	float CrosshairRelativeLocationX = 0.5f;
	UPROPERTY(EditAnywhere)
	float CrosshairRelativeLocationY = 0.33333f;
	UPROPERTY(EditAnywhere)
	float CrosshairMaxRange = 10000.f;
};
