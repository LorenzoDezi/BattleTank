// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameOverDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWonDelegate);


UCLASS()
class BATTLETANK_API AGameManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	void TowerDefeated();
	void PlayerDefeated();
	UPROPERTY(BlueprintAssignable, Category = "Game")
	FGameOverDelegate OnGameOver;
	UPROPERTY(BlueprintAssignable, Category = "Game")
	FWonDelegate OnWon;

private:
	int32 NumberOfTowers = 0;	
	
};
