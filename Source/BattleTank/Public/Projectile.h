// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class UParticleSystemComponent;
class UProjectileMovementComponent;
class URadialForceComponent;

UCLASS()
class BATTLETANK_API AProjectile : public AActor
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Sets default values for this actor's properties
	AProjectile();
	void Launch(float Speed);
	UFUNCTION()
	void OnHit(UPrimitiveComponent * HitComponent, AActor * OtherActor, UPrimitiveComponent * OtherComponent, FVector NormalImpulse, const FHitResult & Hit);
private:
	void SelfDestroy();
	UProjectileMovementComponent * ProjectileMovComponent = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "Setup")
	UStaticMeshComponent * MeshComponent = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "Setup")
	UParticleSystemComponent * LaunchBlast = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "Setup")
	UParticleSystemComponent * ImpactBlast = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "Setup")
	URadialForceComponent * ExplosionForce = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float DestroyDelayAfterHit = 2.f;
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float HitDamage = 20.f;
	
	
};
