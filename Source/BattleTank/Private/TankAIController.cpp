// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "Tank.h"
#include "Engine/World.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Actor.h"

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
	//Check player tank
	ATank* playerTank = GetPlayerTank();
	if (!playerTank) {
		UE_LOG(LogTemp, Error, 
			TEXT("Player tank not found by AI %s"), *this->GetName());
	}
	else {
		UE_LOG(LogTemp, Warning, 
			TEXT("AI %s found player tank %s"), *this->GetName(), *playerTank->GetName());
	}
	//Check controlled tank
	auto controlledTank = GetControlledTank();
	if (!controlledTank) {
		UE_LOG(LogTemp, Error, 
			TEXT("AI not possessing any tank!"));
	}
	else {
		UE_LOG(LogTemp, Warning, 
			TEXT("AI posseessing %s tank"), *controlledTank->GetName());
	}
}

void ATankAIController::Tick(float Deltaseconds)
{
	Super::Tick(Deltaseconds);
	if (!GetControlledTank() || !GetPlayerTank()) return;
	AimAtPlayer();
}

void ATankAIController::AimAtPlayer() {

	FVector HitLocation = GetPlayerTank()->GetTransform().GetLocation();
	GetControlledTank()->AimAt(HitLocation);
}



ATank * ATankAIController::GetPlayerTank() const
{
	auto playerTank = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (playerTank)
		return Cast<ATank>(playerTank);
	else
		return nullptr;
}

ATank* ATankAIController::GetControlledTank() const {
	return Cast<ATank>(GetPawn());
}


