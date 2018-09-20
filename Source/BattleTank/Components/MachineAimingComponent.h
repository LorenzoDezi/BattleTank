// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MachineAimingComponent.generated.h"

UENUM()
enum class EFiringState : uint8 {
	Reloading,
	Aiming,
	Locked,
	OutOfAmmo
};

class UMachineBarrel;
class UMachineTurret;
class AProjectile;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFireDelegate);


//Holds parameters for barrel's properties
UCLASS( meta=(BlueprintSpawnableComponent) )
class BATTLETANK_API UMachineAimingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMachineAimingComponent();
	UFUNCTION(BlueprintCallable, Category = "Setup")
	void Initialise(UMachineBarrel* BarrelToSet, UMachineTurret* TurretToSet);
	void AimAt(FVector AimLocation);
	void AimSuspiciously();
	void RotateTurretTowards(FVector Direction);
	UFUNCTION(BlueprintCallable, Category = "Actions")
	void Fire();
	UFUNCTION(BlueprintPure, Category = "Info")
	const int32 GetCurrentAmmo();
	void PrepareForQuadSpeed(float MaxTimeQuadSpeed);
	UFUNCTION(BlueprintPure, Category = "Info")
	const bool GetCanQuadSpeed();
	UFUNCTION(BlueprintPure, Category = "Info")
	const bool GetIsQuadSpeed();
	UFUNCTION(BlueprintCallable, Category = "Actions")
	void StartQuadSpeed();

	void SetMaxAmmo(int32 MaxAmmo);
	void RecoverAmmo(int32 Ammo);
	void SetTimeToReload(float timeToReloadInSeconds);
	bool IsEnemyInTrajectory(FVector AimLocation);
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
	float LaunchSpeed = 10000; //Sensibile default value of 100000
	float LastFireTime = 0.f;
	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	float TimeToReloadInSeconds = 2.f;
	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	int32 MaxAmmo = -1;
	int32 CurrentAmmo;
	bool CanQuadSpeed = false;
	bool IsQuadSpeed = false;
	float LastTimeQuadSpeed = 0.f;
	float MaxTimeQuadSpeed = 0.f;
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
	UMachineBarrel* Barrel = nullptr;
	UMachineTurret* Turret = nullptr;
	//Used to redirect the barrel to the aim point	
	void MoveBarrelTowards(FVector Direction);
	void CheckAim(float TimeToReload);
};
