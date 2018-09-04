// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Tank.generated.h"

class UTankAimingComponent;
class UTankMovementComponent;
class UPatrolRouteComponent;
class USoundBase;
class UAudioComponent;
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
	void AimAt(FVector AimLocation);
	void BeginPlay() override;
	void SetMaxHealth(int32 MaxHealth);
	UFUNCTION(BlueprintPure)
	float GetHealthPercent() const;
	UFUNCTION(BlueprintPure)
	int32 GetCurrentBoosts() const;
	FDeathDelegate OnDeathDelegate;
	virtual void GetActorEyesViewPoint
	(
		FVector & OutLocation,
		FRotator & OutRotation
	) const override;

private:
	int32 MaxHealth = 100;
	UPROPERTY(VisibleAnywhere, Category = "Health")
	int32 Health = MaxHealth;
	UPROPERTY(EditDefaultsOnly, Category = "Patrol")
	UPatrolRouteComponent* PatrolComponent;
};
