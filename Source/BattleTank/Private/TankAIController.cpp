// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "Tank.h"
#include "Engine/World.h"
#include "TankAimingComponent.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Actor.h"

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
	auto possessedTank = Cast<ATank>(GetPawn());
	if (!ensure(possessedTank)) return;
	possessedTank->SetMaxHealth(MaxHealth);
	auto AimingComponent = possessedTank->FindComponentByClass<UTankAimingComponent>();
	if (!ensure(AimingComponent)) return;
	AimingComponent->SetTimeToReload(TimeToReloadInSeconds);
	EndedSetup();
}

void ATankAIController::Tick(float Deltaseconds)
{
	Super::Tick(Deltaseconds);
	AimAtPlayer();
}

void ATankAIController::OnTankDeath() {
	
	GetPawn()->DetachFromControllerPendingDestroy();
}


void ATankAIController::AimAtPlayer() {

	auto ControlledTank = GetPawn();
	auto PlayerTank = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (ensure(PlayerTank && ControlledTank)) {
		MoveToActor(PlayerTank, AcceptanceRadius);
		FVector HitLocation = PlayerTank->GetTransform().GetLocation();
		auto AimingComponent = ControlledTank->FindComponentByClass<UTankAimingComponent>();
		if (!ensure(AimingComponent)) return;
		AimingComponent->AimAt(HitLocation);
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




