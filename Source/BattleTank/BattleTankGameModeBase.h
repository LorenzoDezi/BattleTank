// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BattleTankGameModeBase.generated.h"

class AMusicManager;

/**
 * 
 */
UCLASS()
class BATTLETANK_API ABattleTankGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AMusicManager * GetMusicManager();
	virtual void BeginPlay() override;
	void IncrementEnemiesAttacking();
	void DecrementEnemiesAttacking();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Manager")
	TSubclassOf<AMusicManager> MusicManagerToSpawn = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = "Music")
	USoundBase* MainMusic = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = "Music")
	USoundBase* BattleMusic = nullptr;
	AMusicManager* MusicManager = nullptr;
	
	
};
