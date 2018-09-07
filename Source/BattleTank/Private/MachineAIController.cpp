// Fill out your copyright notice in the Description page of Project Settings.

#include "MachineAIController.h"
#include "Machine.h"
#include "Engine/World.h"
#include "MachineAimingComponent.h"
#include "Components/ActorComponent.h"
#include "PatrolRouteComponent.h"
#include "GameFramework/Actor.h"

void AMachineAIController::BeginPlay()
{
	Super::BeginPlay();
	
}

AMachineAIController::AMachineAIController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMachineAIController::OnTankDeath() {
	
	GetPawn()->DetachFromControllerPendingDestroy();
}

float AMachineAIController::GetAcceptanceRadius()
{
	return AcceptanceRadius;
}

void AMachineAIController::Possess(APawn * pawn)
{
	Super::Possess(pawn);
	auto possessedTank = Cast<AMachine>(pawn);
	if (!possessedTank) return;
	possessedTank->SetMaxHealth(MaxHealth);
	auto AimingComponent = possessedTank->FindComponentByClass<UMachineAimingComponent>();
	if (!ensure(AimingComponent)) return;
	AimingComponent->SetTimeToReload(TimeToReloadInSeconds);
	EndedSetup();
}

void AMachineAIController::Tick(float DeltaSeconds)
{
	switch (TankAIState) {
	case EMachineAIState::Attacking:
		//TODO
		AimAtPlayer();
		break;
	case EMachineAIState::Suspicious:
		//AimSuspiciously(); going around 90 - -90 degrees
		break;
	case EMachineAIState::Patrolling:
		//AimPatrolling(); forward
		break;
	}
}

void AMachineAIController::SetState(EMachineAIState state)
{
	TankAIState = state;
}

void AMachineAIController::SetEnemy(AActor * enemy)
{
	this->Enemy = enemy;
}

void AMachineAIController::AimAtPlayer() {

	auto ControlledTank = GetPawn();
	if (Enemy && ControlledTank) {
		MoveToActor(Enemy, AcceptanceRadius);
		FVector HitLocation = Enemy->GetTransform().GetLocation();
		LastSeenLocation = HitLocation;
		auto AimingComponent = ControlledTank->FindComponentByClass<UMachineAimingComponent>();
		if (!AimingComponent) return;
		AimingComponent->AimAt(HitLocation);
		UE_LOG(LogTemp, Warning, TEXT(" FIRE AIM AT PLAYER CALLED"));
		if (AimingComponent->GetFiringState() == EFiringState::Locked) {
			AimingComponent->Fire();
		} 
	}
}

void AMachineAIController::SetPawn(APawn * InPawn)
{
	Super::SetPawn(InPawn);
	if (!InPawn) return;
	auto possessedTank = Cast<AMachine>(InPawn);
	if (!ensure(possessedTank)) return;
	possessedTank->OnDeathDelegate.AddUniqueDynamic(this, &AMachineAIController::OnTankDeath);
}




