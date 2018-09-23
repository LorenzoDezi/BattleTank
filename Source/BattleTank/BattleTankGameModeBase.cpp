// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleTankGameModeBase.h"
#include "Actors/Managers/MusicManager.h"
#include "Actors/Managers/GameManager.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/World.h"

AMusicManager* ABattleTankGameModeBase::GetMusicManager() {
	return MusicManager;
}

AGameManager * ABattleTankGameModeBase::GetGameManager()
{
	return GameManager;
}

void ABattleTankGameModeBase::BeginPlay()
{
	if (!MusicManagerToSpawn) return;
	MusicManager = GetWorld()->SpawnActorDeferred<AMusicManager>(MusicManagerToSpawn, FTransform(FVector(0, 0, 0)));
	MusicManager->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	UGameplayStatics::FinishSpawningActor(MusicManager, FTransform(FVector(0, 0, 0)));
	MusicManager->SetMainSound(MainMusic);
	MusicManager->SetBattleSound(BattleMusic);
	if (!GameManagerToSpawn) return;
	GameManager = GetWorld()->SpawnActorDeferred<AGameManager>(GameManagerToSpawn, FTransform(FVector(0, 0, 0)));
	GameManager->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	UGameplayStatics::FinishSpawningActor(GameManager, FTransform(FVector(0, 0, 0)));
	OnManagersCreated.Broadcast();
}

void ABattleTankGameModeBase::IncrementEnemiesAttacking()
{
	if(MusicManager)
		MusicManager->IncrementEnemiesAttacking();
}

void ABattleTankGameModeBase::DecrementEnemiesAttacking()
{
	if(MusicManager)
		MusicManager->DecrementEnemiesAttacking();
}

void ABattleTankGameModeBase::TowerDefeated()
{
	if (GameManager)
		GameManager->TowerDefeated();
}

void ABattleTankGameModeBase::PlayerDefeated()
{
	if (GameManager)
		GameManager->PlayerDefeated();
}
