// Fill out your copyright notice in the Description page of Project Settings.

#include "MusicManager.h"
#include "Components/AudioComponent.h"


// Sets default values
AMusicManager::AMusicManager()
{
	PrimaryActorTick.bCanEverTick = false;
	MainAudioComp = CreateDefaultSubobject<UAudioComponent>(FName("MainAudioComponent"));
	BattleAudioComp = CreateDefaultSubobject<UAudioComponent>(FName("BattleAudioComponent"));
	MainAudioComp->SetAutoActivate(false);
	BattleAudioComp->SetAutoActivate(false);

}

// Called when the game starts or when spawned
void AMusicManager::BeginPlay()
{
	if (Main)
		MainAudioComp->SetSound(Main);
	if (Battle)
		BattleAudioComp->SetSound(Battle);
	Super::BeginPlay();
	MainAudioComp->Play();
}

void AMusicManager::IncrementEnemiesAttacking()
{
	enemiesAttacking++;
	UE_LOG(LogTemp, Warning, TEXT("Number of enemies: %i"), enemiesAttacking);
	if (enemiesAttacking > 0 && GameplayState != EGameplayState::Fighting) {
		UE_LOG(LogTemp, Warning, TEXT("Change of music"));
		MainAudioComp->FadeOut(2.f, 0.5f);
		BattleAudioComp->FadeIn(2.f, 1.f);
		GameplayState = EGameplayState::Fighting;
	}
}

void AMusicManager::DecrementEnemiesAttacking()
{
	enemiesAttacking--;
	UE_LOG(LogTemp, Warning, TEXT("Number of enemies: %i"), enemiesAttacking);
	if (enemiesAttacking == 0 && GameplayState != EGameplayState::Exploring) {
		UE_LOG(LogTemp, Warning, TEXT("Change of music"));
		BattleAudioComp->FadeOut(2.f, 0.5f);
		MainAudioComp->FadeIn(2.f, 1.f);
		GameplayState = EGameplayState::Exploring;
	}
}

void AMusicManager::SetMainSound(USoundBase * main)
{
	Main = main;
	if (Main)
		MainAudioComp->SetSound(Main);
	if (GameplayState == EGameplayState::Exploring) MainAudioComp->Play();
}

void AMusicManager::SetBattleSound(USoundBase* battle)
{
	Battle = battle;
	if (Battle)
		BattleAudioComp->SetSound(Battle);
	if (GameplayState == EGameplayState::Fighting) BattleAudioComp->Play();
}





