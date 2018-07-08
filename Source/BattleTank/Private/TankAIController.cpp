// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "Engine/World.h"
#include "TankAimingComponent.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Actor.h"

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATankAIController::Tick(float Deltaseconds)
{
	Super::Tick(Deltaseconds);
	AimAtPlayer();
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
	}
}




