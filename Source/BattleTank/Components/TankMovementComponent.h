// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/NavMovementComponent.h"
#include "TankMovementComponent.generated.h"

class UTankTrack;
class USoundBase;

/**
 * 
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class BATTLETANK_API UTankMovementComponent : public UNavMovementComponent
{
	GENERATED_BODY()
	
public:
	UTankMovementComponent();
	UFUNCTION(BlueprintCallable, Category = Actions)
	void IntendMoveForward(float Throw);
	UFUNCTION(BlueprintCallable, Category = Actions)
	void IntendTurnRight(float Throw);
	UFUNCTION(BlueprintCallable, Category = Actions)
	void IntendTurnLeft(float Throw);
	UFUNCTION(BlueprintCallable, Category = Actions)
	void Boost(float Throttle);
	float GetBoostPercent() const;
	void BeginPlay() override;
	virtual void TickComponent(
		float DeltaTime,
		ELevelTick TickType,
		FActorComponentTickFunction * ThisTickFunction
	) override;


	void RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed) override;

	UFUNCTION(BlueprintCallable, Category = Setup)
	void Initialise(UTankTrack* leftTrack, UTankTrack* rightTrack);

private:
	UTankTrack* LeftTrack = nullptr;
	UTankTrack* RightTrack = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = "Boost")
	int32 MaxNumberOfBoosts = 2;
	int32 NumberOfBoosts = 2;
	UPROPERTY(EditDefaultsOnly, Category = "Boost")
	float TimeToRecoverBoosts = 3.f;
	float LastTimeUsedBoost = 0.f;
	UPROPERTY(EditDefaultsOnly, Category = "Boost")
	USoundBase* TankBoost = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = "Boost")
	USoundBase* TankBoostEmpty = nullptr;
};
