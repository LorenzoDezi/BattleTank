// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SprungWheel.generated.h"

class UPhysicsConstraintComponent;
class USphereComponent;

UCLASS()
class BATTLETANK_API ASprungWheel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASprungWheel();
	void AddDrivingForce(float ForceMagnitude);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(
		float DeltaTime
	) override;

private:
	UPROPERTY(VisibleAnywhere, Category = Components)
	UPhysicsConstraintComponent * Spring = nullptr;
	UPROPERTY(VisibleAnywhere, Category = Components)
	USphereComponent * Axis = nullptr;
	UPROPERTY(VisibleAnywhere, Category = Components)
	UPhysicsConstraintComponent * WheelAxisConstraint = nullptr;
	UPROPERTY(VisibleAnywhere, Category = Components)
	USphereComponent * Wheel = nullptr;
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, 
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp, 
		FVector NormalImpulse, 
		const FHitResult& Hit);
	float CurrentForceToApply = 0.f;
	void ApplyForce();

	
	
};
