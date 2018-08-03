// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/NavMovementComponent.h"
#include "TankMovementComponent.generated.h"

class UTankTrack;

/**
 * 
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class BATTLETANK_API UTankMovementComponent : public UNavMovementComponent
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = Actions)
	void IntendMoveForward(float Throw);
	UFUNCTION(BlueprintCallable, Category = Actions)
	void IntendTurnRight(float Throw);
	UFUNCTION(BlueprintCallable, Category = Actions)
	void IntendTurnLeft(float Throw);
	UFUNCTION(BlueprintCallable, Category = Actions)
	void Boost(float Throttle);

	void RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed) override;

	UFUNCTION(BlueprintCallable, Category = Setup)
	void Initialise(UTankTrack* leftTrack, UTankTrack* rightTrack);

private:
	UTankTrack* LeftTrack = nullptr;
	UTankTrack* RightTrack = nullptr;

	
};
