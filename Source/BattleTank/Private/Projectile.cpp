// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("Projectile Mesh Component"));
	SetRootComponent(MeshComponent);
	MeshComponent->SetNotifyRigidBodyCollision(true);
	MeshComponent->SetVisibility(true);
	LaunchBlast = CreateDefaultSubobject<UParticleSystemComponent>(FName("Projectile Particle Component"));
	ProjectileMovComponent =  CreateDefaultSubobject<UProjectileMovementComponent>(FName("Projectile Movement Component"));
	ProjectileMovComponent->bAutoActivate = false;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::Launch(float Speed)
{
	if (!ProjectileMovComponent) return;
	ProjectileMovComponent->SetVelocityInLocalSpace(FVector::ForwardVector * Speed);
	ProjectileMovComponent->Activate();
}

