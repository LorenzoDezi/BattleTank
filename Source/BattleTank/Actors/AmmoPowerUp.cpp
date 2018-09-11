// Fill out your copyright notice in the Description page of Project Settings.

#include "AmmoPowerUp.h"
#include "Machine.h"

void AAmmoPowerUp::PowerUp(AMachine * machine)
{
	machine->RecoverAmmo();
}
