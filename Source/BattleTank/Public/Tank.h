// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Tank.generated.h"

class UTankAimingComponent;
class AProjectile;

UCLASS()
class BATTLETANK_API ATank : public APawn
{
	GENERATED_BODY()
public:
	// Sets default values for this pawn's properties
	ATank();
	UFUNCTION(BlueprintCallable, Category=Setup)
	void SetBarrel(UTankBarrel* BarrelToSet);
	UFUNCTION(BlueprintCallable, Category = Setup)
	void SetTurret(UTankTurret* TurretToSet);
	UFUNCTION(BlueprintCallable, Category = Actions)
	void Fire();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UTankAimingComponent* AimingComponent;

public:	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void AimAt(FVector AimLocation);

private:
	UPROPERTY(EditAnywhere, Category = Firing)
	float LaunchSpeed = 100000; //Sensibile default value of 100000
	UPROPERTY(EditAnywhere, Category = Firing)
	TSubclassOf<AProjectile> Projectile = nullptr;
	UTankBarrel* Barrel = nullptr;

};
