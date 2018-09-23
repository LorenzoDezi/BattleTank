// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BattleTankGameModeBase.generated.h"

class AMusicManager;
class AGameManager;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FManagerCreated);


/**
 * 
 */
UCLASS()
class BATTLETANK_API ABattleTankGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category="Manager")
	AMusicManager * GetMusicManager();
	UFUNCTION(BlueprintCallable, Category = "Manager")
	AGameManager* GetGameManager();
	virtual void BeginPlay() override;
	void IncrementEnemiesAttacking();
	void DecrementEnemiesAttacking();
	void TowerDefeated();
	void PlayerDefeated();
	UPROPERTY(BlueprintAssignable, Category = "Manager")
	FManagerCreated OnManagersCreated;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Manager")
	TSubclassOf<AMusicManager> MusicManagerToSpawn = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = "Manager")
	TSubclassOf<AGameManager> GameManagerToSpawn = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = "Music")
	USoundBase* MainMusic = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = "Music")
	USoundBase* BattleMusic = nullptr;
	AMusicManager* MusicManager = nullptr;
	AGameManager* GameManager = nullptr;
	
	
};
