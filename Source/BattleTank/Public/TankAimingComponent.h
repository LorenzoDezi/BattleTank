// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TankAimingComponent.generated.h"

UENUM()
enum class EFiringState : uint8 {
	Reloading,
	Aiming,
	Locked,
	OutOfAmmo
};

class UTankBarrel;
class UTankTurret;
class AProjectile;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFireDelegate);


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
	UFUNCTION(BlueprintPure, Category = "Info")
	const int32 GetCurrentAmmo();
	void SetMaxAmmo(int32 MaxAmmo);
	void SetTimeToReload(float timeToReloadInSeconds);
	const EFiringState GetFiringState();

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
	int32 MaxAmmo = -1;
	int32 CurrentAmmo;
	UPROPERTY(BlueprintAssignable, Category = "Firing")
	FFireDelegate OnFireDelegate;
	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
	USoundBase* TankFire;
	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
	USoundBase* TankOutOfAmmo;
	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	TSubclassOf<AProjectile> Projectile = nullptr;
	//Used to check for the aiming FiringState
	FVector AimDirection = FVector();
	UTankBarrel* Barrel = nullptr;
	UTankTurret* Turret = nullptr;
	//Used to redirect the barrel to the aim point	
	void MoveBarrelTowards(FVector Direction);	
};
