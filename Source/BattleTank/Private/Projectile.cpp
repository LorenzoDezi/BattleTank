// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"

// Sets default values
AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	//Setting mesh
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("Projectile Mesh Component"));
	SetRootComponent(MeshComponent);
	MeshComponent->SetNotifyRigidBodyCollision(true);
	MeshComponent->SetVisibility(false);

	//Setting particle systems
	LaunchBlast = CreateDefaultSubobject<UParticleSystemComponent>(FName("Launch Blast"));
	LaunchBlast->SetupAttachment(RootComponent);
	ImpactBlast = CreateDefaultSubobject<UParticleSystemComponent>(FName("Impact Blast"));
	ImpactBlast->SetupAttachment(RootComponent);
	ImpactBlast->bAutoActivate = false;

	//Setting movement component
	ProjectileMovComponent = CreateDefaultSubobject<UProjectileMovementComponent>(FName("Projectile Movement Component"));
	ProjectileMovComponent->bAutoActivate = false;

}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	MeshComponent->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
	Super::BeginPlay();
}

void AProjectile::Launch(float Speed)
{
	UE_LOG(LogTemp, Warning, TEXT("Inside launch, at %f"), Speed);
	if (!ProjectileMovComponent) return;
	UE_LOG(LogTemp, Warning, TEXT("After check projectile %s"), *ProjectileMovComponent->GetName());
	ProjectileMovComponent->SetVelocityInLocalSpace(FVector::ForwardVector * Speed);
	ProjectileMovComponent->Activate();
}

void AProjectile::OnHit(UPrimitiveComponent * HitComponent, AActor * OtherActor, UPrimitiveComponent * OtherComponent, FVector NormalImpulse, const FHitResult & Hit)
{
	LaunchBlast->Deactivate();
	ImpactBlast->Activate();
}

