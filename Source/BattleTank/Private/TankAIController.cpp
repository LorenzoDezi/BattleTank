// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "Tank.h"
#include "Engine/World.h"
#include "TankAimingComponent.h"
#include "Components/ActorComponent.h"
#include "PatrolRouteComponent.h"
#include "GameFramework/Actor.h"

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
	
}

ATankAIController::ATankAIController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATankAIController::OnTankDeath() {
	
	GetPawn()->DetachFromControllerPendingDestroy();
}

float ATankAIController::GetAcceptanceRadius()
{
	return AcceptanceRadius;
}

void ATankAIController::Possess(APawn * pawn)
{
	Super::Possess(pawn);
	auto possessedTank = Cast<ATank>(pawn);
	if (!possessedTank) return;
	possessedTank->SetMaxHealth(MaxHealth);
	auto AimingComponent = possessedTank->FindComponentByClass<UTankAimingComponent>();
	if (!ensure(AimingComponent)) return;
	AimingComponent->SetTimeToReload(TimeToReloadInSeconds);
	EndedSetup();
}

void ATankAIController::Tick(float DeltaSeconds)
{
	switch (TankAIState) {
	case ETankAIState::Attacking:
		//TODO
		AimAtPlayer();
		break;
	case ETankAIState::Suspicious:
		//AimSuspiciously(); going around 90 - -90 degrees
		break;
	case ETankAIState::Patrolling:
		//AimPatrolling(); forward
		break;
	}
}

void ATankAIController::SetState(ETankAIState state)
{
	TankAIState = state;
}

void ATankAIController::SetEnemy(AActor * enemy)
{
	this->Enemy = enemy;
}

void ATankAIController::AimAtPlayer() {

	auto ControlledTank = GetPawn();
	if (Enemy && ControlledTank) {
		MoveToActor(Enemy, AcceptanceRadius);
		FVector HitLocation = Enemy->GetTransform().GetLocation();
		LastSeenLocation = HitLocation;
		auto AimingComponent = ControlledTank->FindComponentByClass<UTankAimingComponent>();
		if (!AimingComponent) return;
		AimingComponent->AimAt(HitLocation);
		UE_LOG(LogTemp, Warning, TEXT(" FIRE AIM AT PLAYER CALLED"));
		if (AimingComponent->GetFiringState() == EFiringState::Locked) {
			AimingComponent->Fire();
		} 
	}
}

void ATankAIController::SetPawn(APawn * InPawn)
{
	Super::SetPawn(InPawn);
	if (!InPawn) return;
	auto possessedTank = Cast<ATank>(InPawn);
	if (!ensure(possessedTank)) return;
	possessedTank->OnDeathDelegate.AddUniqueDynamic(this, &ATankAIController::OnTankDeath);
}




