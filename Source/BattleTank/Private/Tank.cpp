
#include "Tank.h"
#include "Projectile.h"
#include "Engine/World.h"
#include "Runtime/Engine/Classes/Components/AudioComponent.h"
#include "Engine/StaticMeshSocket.h"
#include "PatrolRouteComponent.h"
#include "TankBarrel.h"
#include "TankAimingComponent.h"
#include "TankMovementComponent.h"



// Sets default values
ATank::ATank()
{
	PrimaryActorTick.bCanEverTick = false;
	//Setting the components
	RootComponent = CreateDefaultSubobject<USceneComponent>(FName("Root"));
	UStaticMeshComponent* Mesh = FindComponentByClass<UStaticMeshComponent>();
	if(Mesh)
		Mesh->AttachTo(RootComponent);
	PatrolComponent = CreateDefaultSubobject<UPatrolRouteComponent>(FName("PatrolRouteComponent"));
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(FName("AudioComponent"));
	AudioComponent->AttachTo(RootComponent);
	if(AudioComponent)
		AudioComponent->bAutoActivate = false;
}

void ATank::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (AudioComponent && TankLoop) {
		AudioComponent->SetSound(TankLoop);
	}
}

void ATank::BeginPlay()
{
	Super::BeginPlay();
	if (!AudioComponent) return;
	AudioComponent->Play();
}

float ATank::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, 
	AController * EventInstigator, AActor * DamageCauser)
{
	//The tank is already dead
	if (!Health) {
		return 0.f;
	}
	//Apply damage
	float DamageApplied = 0.f;
	if (Health - DamageAmount < 0) {
		DamageApplied = Health;
		Health = 0;
	}
	else  {
		Health -= DamageAmount;
		DamageApplied = DamageAmount;
	}
	//Broadcast death event if dead after the damage
	if (!Health)
		OnDeathDelegate.Broadcast();
	return DamageApplied;
}

void ATank::AimAt(FVector AimLocation)
{
	auto AimComponent = this->FindComponentByClass<UTankAimingComponent>();
	if (AimComponent) {
		AimComponent->AimAt(AimLocation);
	}
}



void ATank::SetMaxHealth(int32 MaxHealth)
{
	this->MaxHealth = MaxHealth;
	Health = MaxHealth;
}

float ATank::GetHealthPercent() const
{
	return (float)Health / (float)MaxHealth;
}

int32 ATank::GetCurrentBoosts() const
{
	UTankMovementComponent* TankMovComp = FindComponentByClass<UTankMovementComponent>();
	if (!TankMovComp) return 0;
	else
		return TankMovComp->GetCurrentBoosts();
}
