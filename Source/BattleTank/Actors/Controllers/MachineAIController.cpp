// Fill out your copyright notice in the Description page of Project Settings.

#include "Actors/Controllers/MachineAIController.h"
#include "Actors/Machine.h"
#include "BattleTankGameModeBase.h"
#include "Engine/World.h"
#include "Components/MachineAimingComponent.h"
#include "Components/ActorComponent.h"
#include "Components/PatrolRouteComponent.h"
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
		AimAtPlayer();
		break;
	case EMachineAIState::Suspicious:
		AimSuspiciously();
		break;
	case EMachineAIState::Patrolling:
		AimPatrolling();
		break;
	}
}

void AMachineAIController::SetState(EMachineAIState state)
{
	auto gameMode = GetWorld()->GetAuthGameMode();
	ABattleTankGameModeBase* battleTankMode = nullptr;
	if (gameMode->IsA<ABattleTankGameModeBase>()) {
		battleTankMode = Cast<ABattleTankGameModeBase>(gameMode);
		if (GetState() == EMachineAIState::Attacking && state != EMachineAIState::Attacking) {
			battleTankMode->DecrementEnemiesAttacking();
		}
		else if (GetState() != EMachineAIState::Attacking && state == EMachineAIState::Attacking) {
			battleTankMode->IncrementEnemiesAttacking();
		}
	}
	TankAIState = state;
}

EMachineAIState AMachineAIController::GetState()
{
	return TankAIState;
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

void AMachineAIController::AimSuspiciously() {

	auto ControlledTank = GetPawn();
	if (!ControlledTank) return;
	auto AimingComponent = ControlledTank->FindComponentByClass<UMachineAimingComponent>();
	if (!AimingComponent) return;
	AimingComponent->AimSuspiciously();
}

void AMachineAIController::AimPatrolling() {
	auto ControlledTank = GetPawn();
	if (!ControlledTank) return;
	auto AimingComponent = ControlledTank->FindComponentByClass<UMachineAimingComponent>();
	if (!AimingComponent) return;
	if(!GetPawn()) return
	AimingComponent->RotateTurretTowards(GetPawn()->GetActorForwardVector());
}


void AMachineAIController::SetPawn(APawn * InPawn)
{
	Super::SetPawn(InPawn);
	if (!InPawn) return;
	auto possessedTank = Cast<AMachine>(InPawn);
	if (!ensure(possessedTank)) return;
	possessedTank->OnDeathDelegate.AddUniqueDynamic(this, &AMachineAIController::OnTankDeath);
}




