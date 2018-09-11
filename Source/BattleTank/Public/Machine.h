// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "WidgetAssociatedActor.h"
#include "Machine.generated.h"

class UMachineAimingComponent;
class UTankMovementComponent;
class UPatrolRouteComponent;
class USoundBase;
class UAudioComponent;
class UBlackboardComponent;
class ATower;
class AProjectile;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDeathDelegate);

UCLASS()
class BATTLETANK_API AMachine : public APawn, public IWidgetAssociatedActor
{
	GENERATED_BODY()
public:
	// Sets default values for this pawn's properties
	AMachine();
	virtual float TakeDamage
	(
		float DamageAmount,
		struct FDamageEvent const & DamageEvent,
		class AController * EventInstigator,
		AActor * DamageCauser
	) override;
	void AimAt(FVector AimLocation);
	UFUNCTION()
	void OnMotherTowerAlarm(AActor* attacker);
	void BeginPlay() override;
	void SetMaxHealth(int32 MaxHealth);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Health")
	float GetHealthPercent() const;
	virtual float GetHealthPercent_Implementation() const;	
	UFUNCTION(BlueprintPure)
	float GetBoostPercent() const;
	FDeathDelegate OnDeathDelegate;
	virtual void GetActorEyesViewPoint
	(
		FVector & OutLocation,
		FRotator & OutRotation
	) const override;

private:
	int32 MaxHealth = 100;
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float SecondsToDestroyAfterDeath = 10.f;
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	USoundBase* TankDestroyedSound;
	UPROPERTY(VisibleAnywhere, Category = "Health")
	int32 Health = MaxHealth;
	UPROPERTY(EditDefaultsOnly, Category = "Patrol")
	UPatrolRouteComponent* PatrolComponent;

	UFUNCTION()
	//Destroy the object for setTimer use
	void DestroyCall();
	//Destroy all components and particles, and enable physics
	//to give ragdoll effect to the defeated tank
	void Disassemble();
	UBlackboardComponent* GetBlackboard();

};
