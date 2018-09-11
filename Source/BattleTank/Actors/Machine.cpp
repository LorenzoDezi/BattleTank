
#include "Actors/Machine.h"
#include "Actors/Projectile.h"
#include "Actors/Tower.h"
#include "Engine/World.h"
#include "Runtime/Engine/Classes/Components/AudioComponent.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "Runtime/Engine/Classes/Components/AudioComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Actors/Controllers/MachineAIController.h"
#include "Engine/StaticMeshSocket.h"
#include "Components/PatrolRouteComponent.h"
#include "Components/MachineBarrel.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Components/MachineAimingComponent.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Components/MachineTurret.h"
#include "Components/TankMovementComponent.h"



// Sets default values
AMachine::AMachine()
{
	PrimaryActorTick.bCanEverTick = false;
	//Setting the components
	UStaticMeshComponent* Mesh = FindComponentByClass<UStaticMeshComponent>();
	RootComponent = Mesh;	
}

void AMachine::BeginPlay()
{
	Super::BeginPlay();
	UPatrolRouteComponent* patrolComponent = FindComponentByClass<UPatrolRouteComponent>();
	if (!patrolComponent) return;
	TArray<AActor*> patrolPoints = TArray<AActor*>();
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Tower"), patrolPoints);
	patrolComponent->SetPatrolPoints(patrolPoints);
}

float AMachine::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, 
	AController * EventInstigator, AActor * DamageCauser)
{
	if (DamageCauser) {
		//Refactor in a function
		auto Blackboard = GetBlackboard();
		if(!DamageCauser->ActorHasTag(FName("Enemy")) && Blackboard)
			Blackboard->SetValueAsObject(FName("PlayerTank"), DamageCauser);
	}
	//The tank is already dead
	if (!Health) {
		return 0.f;
	}
	//Apply damage
	float DamageApplied = 0.f;
	if (Health - DamageAmount <= 0) {
		DamageApplied = Health;
		Health = 0;
		Disassemble();
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

void AMachine::AimAt(FVector AimLocation)
{
	auto AimComponent = this->FindComponentByClass<UMachineAimingComponent>();
	if (AimComponent) {
		AimComponent->AimAt(AimLocation);
	}
}

void AMachine::OnMotherTowerAlarm(AActor* attacker)
{
	auto Blackboard = GetBlackboard();
	if(Blackboard)
		Blackboard->SetValueAsObject(FName("PlayerTank"), attacker);
}

void AMachine::RecoverHealth()
{
	if (MaxHealth - Health < HealthRegen)
		Health += HealthRegen;
	else
		Health = MaxHealth;
}

void AMachine::RecoverAmmo()
{
	auto AimComp = this->FindComponentByClass<UMachineAimingComponent>();
	if(AimComp)
		AimComp->RecoverAmmo(AmmoRegen);
}

void AMachine::StartQuadSpeed()
{
	auto AimComp = this->FindComponentByClass<UMachineAimingComponent>();
	if (AimComp)
		AimComp->StartQuadSpeed(MaxTimeQuadSpeed);
}



void AMachine::SetMaxHealth(int32 MaxHealth)
{
	this->MaxHealth = MaxHealth;
	Health = MaxHealth;
}

float AMachine::GetHealthPercent_Implementation() const
{
	return (float)Health / (float)MaxHealth;
}

float AMachine::GetBoostPercent() const
{
	UTankMovementComponent* TankMovComp = FindComponentByClass<UTankMovementComponent>();
	if (!TankMovComp) return 0;
	else
		return TankMovComp->GetBoostPercent();
}

void AMachine::GetActorEyesViewPoint(FVector & OutLocation, FRotator & OutRotation) const
{
	auto Mesh = this->FindComponentByClass<UMachineTurret>();
	if (!Mesh) {
		return Super::GetActorEyesViewPoint(OutLocation, OutRotation); 
	}
	if (!Mesh->GetSocketByName(FName("ViewPoint"))) return Super::GetActorEyesViewPoint(OutLocation, OutRotation);
	OutLocation = Mesh->GetSocketLocation(FName("ViewPoint"));
	OutRotation = Mesh->GetSocketRotation(FName("ViewPoint"));
}

void AMachine::DestroyCall()
{
	if (this->IsValidLowLevel())
		this->Destroy();
}

void AMachine::Disassemble()
{
	//Distruggo gli actors associati (SprungWheels)
	TArray<AActor*> actors = TArray<AActor*>();
	GetAttachedActors(actors);
	for (AActor* actor : actors) {
		actor->Destroy();
	}
	//Fermo il suono del motore
	UAudioComponent* audioComp = FindComponentByClass<UAudioComponent>();
	if(audioComp)
		audioComp->Stop();
	if (TankDestroyedSound)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), TankDestroyedSound,
			GetActorLocation());
	//Distruggo i particles associati 
	TArray<UActorComponent*> particles
		= GetComponentsByClass(UParticleSystemComponent::StaticClass());
	for (UActorComponent* comp : particles) {
		comp->DestroyComponent();
	}
	//Abilito la fisica sulle meshes
	TArray<UActorComponent*> meshes = GetComponentsByClass(UStaticMeshComponent::StaticClass());
	for (UActorComponent* mesh : meshes) {
		Cast<UMeshComponent>(mesh)->SetSimulatePhysics(true);
	}
	//Distruggo l'attore dopo un tot TODO
	FTimerHandle Timer;
	GetWorldTimerManager().SetTimer(Timer, this, &AMachine::DestroyCall, SecondsToDestroyAfterDeath);
}

UBlackboardComponent * AMachine::GetBlackboard()
{
	auto controller = this->GetController();
	if (!controller) return nullptr;
	AMachineAIController* AIController = nullptr;
	if (controller->IsA(AMachineAIController::StaticClass()))
		AIController = Cast<AMachineAIController>(controller);
	if (!AIController) return nullptr;
	auto Blackboard = AIController->GetBlackboardComponent();
	return Blackboard;
}
