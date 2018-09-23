// Fill out your copyright notice in the Description page of Project Settings.

#include "GameManager.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGameManager::AGameManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AGameManager::BeginPlay()
{
	TArray<AActor*> towers = TArray<AActor*>();
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Tower"), towers);
	NumberOfTowers = towers.Num();
	Super::BeginPlay();
}

void AGameManager::TowerDefeated()
{
	NumberOfTowers--;
	if (NumberOfTowers <= 0) OnWon.Broadcast();
}

void AGameManager::PlayerDefeated()
{
	OnGameOver.Broadcast();
}


