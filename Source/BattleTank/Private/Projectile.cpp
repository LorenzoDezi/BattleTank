// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile.h"
#include "Engine/World.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Runtime/Engine/Classes/PhysicsEngine/RadialForceComponent.h"

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
	ExplosionForce = CreateDefaultSubobject<URadialForceComponent>(FName("Explosion Force"));
	ExplosionForce->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	ExplosionForce->bAutoActivate = false;
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
	if (!ProjectileMovComponent) return;
	ProjectileMovComponent->SetVelocityInLocalSpace(FVector::ForwardVector * Speed);
	ProjectileMovComponent->Activate();
}

void AProjectile::SelfDestroy() {
	Destroy();
}

void AProjectile::OnHit(UPrimitiveComponent * HitComponent, AActor * OtherActor, UPrimitiveComponent * OtherComponent, FVector NormalImpulse, const FHitResult & Hit)
{
	LaunchBlast->Deactivate();
	ImpactBlast->Activate();
	ExplosionForce->FireImpulse();
	
	UGameplayStatics::ApplyRadialDamage(
		this, 
		HitDamage, 
		GetActorLocation(), 
		ExplosionForce->Radius, 
		UDamageType::StaticClass(), 
		TArray<AActor*>());

	MeshComponent->DestroyComponent();
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AProjectile::SelfDestroy, DestroyDelayAfterHit, false);
}

