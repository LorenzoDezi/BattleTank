// Fill out your copyright notice in the Description page of Project Settings.

#include "HealthPowerUp.h"
#include "Machine.h"

void AHealthPowerUp::PowerUp(AMachine* machine) {
	machine->RecoverHealth();
}




