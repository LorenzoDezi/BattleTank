// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "Tank.h"
#include "Engine/World.h"
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

	auto ControlledTank = Cast<ATank>(GetPawn());
	auto PlayerTank = Cast<ATank>(GetWorld()->GetFirstPlayerController()->GetPawn());
	FVector HitLocation = PlayerTank->GetTransform().GetLocation();
	ControlledTank->AimAt(HitLocation);
	//DEBUG
	//ControlledTank->Fire();
}




