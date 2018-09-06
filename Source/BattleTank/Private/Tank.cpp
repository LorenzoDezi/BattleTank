
#include "Tank.h"
#include "Projectile.h"
#include "Tower.h"
#include "Engine/World.h"
#include "Runtime/Engine/Classes/Components/AudioComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TankAIController.h"
#include "Engine/StaticMeshSocket.h"
#include "PatrolRouteComponent.h"
#include "TankBarrel.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "TankAimingComponent.h"
#include "TankTurret.h"
#include "TankMovementComponent.h"



// Sets default values
ATank::ATank()
{
	PrimaryActorTick.bCanEverTick = false;
	//Setting the components
	UStaticMeshComponent* Mesh = FindComponentByClass<UStaticMeshComponent>();
	RootComponent = Mesh;	
}

void ATank::BeginPlay()
{
	Super::BeginPlay();
	UPatrolRouteComponent* patrolComponent = FindComponentByClass<UPatrolRouteComponent>();
	if (!patrolComponent) return;
	TArray<AActor*> patrolPoints = TArray<AActor*>();
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Tower"), patrolPoints);
	patrolComponent->SetPatrolPoints(patrolPoints);
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

void ATank::OnMotherTowerAlarm(ATower* tower)
{
	auto controller = this->GetController();
	if (!controller) return;
	ATankAIController* AIController = nullptr;
	if (controller->IsA(ATankAIController::StaticClass()))
		AIController = Cast<ATankAIController>(controller);
	if (!AIController) return;
	auto Blackboard = AIController->GetBlackboardComponent();
	if (!Blackboard) return;
	Blackboard->SetValueAsObject(FName("MotherTower"), tower);
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

void ATank::GetActorEyesViewPoint(FVector & OutLocation, FRotator & OutRotation) const
{
	auto Mesh = this->FindComponentByClass<UTankTurret>();
	if (!Mesh) {
		return Super::GetActorEyesViewPoint(OutLocation, OutRotation); 
	}
	if (!Mesh->GetSocketByName(FName("ViewPoint"))) return Super::GetActorEyesViewPoint(OutLocation, OutRotation);
	OutLocation = Mesh->GetSocketLocation(FName("ViewPoint"));
	OutRotation = Mesh->GetSocketRotation(FName("ViewPoint"));
}
