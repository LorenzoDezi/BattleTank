// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h" 

class ATank;
class UMachineAimingComponent;

UCLASS()
class BATTLETANK_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	
	void BeginPlay() override;
	virtual void Tick(float Deltaseconds) override;
	UFUNCTION(BlueprintImplementableEvent, Category = "Setup")
	void FoundAimingComponent(UMachineAimingComponent* component);
	UFUNCTION(BlueprintImplementableEvent, Category = "Setup")
	void EndedSetup();

private:
	void AimAtCrosshair();
	bool GetCrosshairLocation(FVector& OutHitLocation) const;
	virtual void SetPawn(APawn* InPawn) override;
	UFUNCTION()
	void OnTankDeath();
	UPROPERTY(EditDefaultsOnly)
	float CrosshairRelativeLocationX = 0.5f;
	UPROPERTY(EditDefaultsOnly)
	float CrosshairRelativeLocationY = 0.33333f;
	UPROPERTY(EditDefaultsOnly)
	float CrosshairMaxRange = 10000.f;
	UPROPERTY(EditDefaultsOnly, Category = "Game Variables")
	int32 MaxHealth = 100.f;
	UPROPERTY(EditDefaultsOnly, Category = "Game Variables")
	int32 MaxAmmo = 50.f;
	UPROPERTY(EditDefaultsOnly, Category = "Game Variables")
	float TimeToReloadInSeconds = 1.f;
};
