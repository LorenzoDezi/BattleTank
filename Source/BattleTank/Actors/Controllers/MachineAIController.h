// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MachineAIController.generated.h"


UENUM()
enum class EMachineAIState : uint8 {
	Attacking,
	Suspicious,
	Patrolling,
	Dead
};

class AActor;

/**
 * 
 */
UCLASS()
class BATTLETANK_API AMachineAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;	
	AMachineAIController();
	UFUNCTION(BlueprintImplementableEvent, Category = "Setup")
	void EndedSetup();
	float GetAcceptanceRadius();
	virtual void Possess(APawn* pawn) override;
	virtual void Tick
	(
		float DeltaSeconds
	) override;
	void SetState(EMachineAIState state);
	EMachineAIState GetState();
	void SetEnemy(AActor* enemy);
private:
	void AimAtPlayer();
	void AimSuspiciously();
	void AimPatrolling();
	UFUNCTION()
	void OnTankDeath();
	virtual void SetPawn(APawn* InPawn) override;
	UPROPERTY(EditDefaultsOnly, Category = Movement)
	float AcceptanceRadius = 5.f;
	UPROPERTY(EditDefaultsOnly, Category = "Game Variables")
	int32 MaxHealth = 100.f;
	UPROPERTY(EditDefaultsOnly, Category = "Game Variables")
	float TimeToReloadInSeconds = 2.f;

	EMachineAIState TankAIState = EMachineAIState::Patrolling;
	AActor* Enemy = nullptr;
	FVector LastSeenLocation = FVector();
	
};
