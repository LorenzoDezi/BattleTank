// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayerController.h"

ATank* ATankPlayerController::GetControlledTank() const {
	return Cast<ATank>(GetPawn());
}

void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();

	auto controlledTank = GetControlledTank();
	if (!controlledTank) {
		UE_LOG(LogTemp, Error, TEXT("Player not possessing any tank!"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Player posseessing %s tank"), *controlledTank->GetName());
	}
}
