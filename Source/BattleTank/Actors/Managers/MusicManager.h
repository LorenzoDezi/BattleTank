// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MusicManager.generated.h"

class USoundBase;
class UAudioComponent;

UENUM()
enum class EGameplayState : uint8 {
	Fighting,
	Exploring,
	GameOver,
	Won
};

UCLASS()
class BATTLETANK_API AMusicManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMusicManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	void IncrementEnemiesAttacking();
	void DecrementEnemiesAttacking();
	void SetMainSound(USoundBase* main);
	void SetBattleSound(USoundBase* battle);

private:
	//Setup
	USoundBase* Main = nullptr;
	USoundBase* Battle = nullptr;
	UAudioComponent* MainAudioComp = nullptr;
	UAudioComponent* BattleAudioComp = nullptr;
	//Logic
	//Keep tracks of enemies attacking the player
	int32 enemiesAttacking = 0;
	EGameplayState GameplayState = EGameplayState::Exploring;
};
