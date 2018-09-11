// Fill out your copyright notice in the Description page of Project Settings.

#include "QuadSpeedPowerUp.h"
#include "Machine.h"

void AQuadSpeedPowerUp::PowerUp(AMachine * machine)
{
	machine->StartQuadSpeed();
}
