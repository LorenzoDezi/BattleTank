// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "TankTrack.generated.h"

/**
 * 
 */
class ASprungWheel;

UCLASS(meta = (BlueprintSpawnableComponent))
class BATTLETANK_API UTankTrack : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
	UTankTrack();
	UFUNCTION(BlueprintCallable, Category = Input)
	void SetThrottle(float Throttle);
	void DriveTrack(float Throttle);
	virtual void BeginPlay() override;
	virtual void TickComponent(
		float DeltaTime, 
		ELevelTick TickType, 
		FActorComponentTickFunction * ThisTickFunction
	) override;


private:
	UPROPERTY(EditDefaultsOnly, Category = Movement)
	float MaxDrivingForce = 40000000; //Mass (4000) per acceleration (1g) per manual fixing
	TArray<class ASprungWheel*> GetWheels() const;
};
