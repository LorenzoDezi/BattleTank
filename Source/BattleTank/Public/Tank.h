// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Tank.generated.h"

class UTankAimingComponent;
class UTankMovementComponent;
class AProjectile;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDeathDelegate);

UCLASS()
class BATTLETANK_API ATank : public APawn
{
	GENERATED_BODY()
public:
	// Sets default values for this pawn's properties
	ATank();
	virtual float TakeDamage
	(
		float DamageAmount,
		struct FDamageEvent const & DamageEvent,
		class AController * EventInstigator,
		AActor * DamageCauser
	) override;
	UFUNCTION(BlueprintPure)
	float GetHealthPercent() const;
	FDeathDelegate OnDeathDelegate;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	int32 MaxHealth = 100;
	UPROPERTY(VisibleAnywhere, Category = "Stats")
	int32 Health = MaxHealth;
};
