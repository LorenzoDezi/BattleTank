// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TankAimingComponent.generated.h"

UENUM()
enum class EFiringState : uint8 {
	Reloading,
	Aiming,
	Locked
};

class UTankBarrel;
class UTankTurret;
class AProjectile;

//Holds parameters for barrel's properties
UCLASS( meta=(BlueprintSpawnableComponent) )
class BATTLETANK_API UTankAimingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTankAimingComponent();
	UFUNCTION(BlueprintCallable, Category = "Setup")
	void Initialise(UTankBarrel* BarrelToSet, UTankTurret* TurretToSet);
	void AimAt(FVector AimLocation);
	UFUNCTION(BlueprintCallable, Category = "Actions")
	void Fire();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void TickComponent
	(
		float DeltaTime,
		enum ELevelTick TickType,
		FActorComponentTickFunction * ThisTickFunction
	) override;
	UPROPERTY(BlueprintReadOnly, Category = "State")
	EFiringState FiringState = EFiringState::Aiming;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	float LaunchSpeed = 100000; //Sensibile default value of 100000
	float LastFireTime = 0.f;
	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	float TimeToReloadInSeconds = 2.f;
	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	TSubclassOf<AProjectile> Projectile = nullptr;
	//Used to check for the aiming FiringState
	FVector AimDirection = FVector();
	UTankBarrel* Barrel = nullptr;
	UTankTurret* Turret = nullptr;
	//Used to redirect the barrel to the aim point	
	void MoveBarrelTowards(FVector Direction);	
};
