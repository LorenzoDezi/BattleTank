// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PowerUp.generated.h"

UCLASS()
class BATTLETANK_API APowerUp : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APowerUp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	//Function called when the power up collides with another actor. Call the superclass first!
	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			FVector NormalImpulse,
			const FHitResult& Hit);
	//Returns the time remaining to enable the powerup, 0.0 means loaded.
	float GetEnabledPercent();
	bool IsEnabled();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float RotationDegreePerSecond = 5.f;
	float LastTimeHit = 0.f;
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float TimeToEnable = 10.f;
	bool enabled = true;
	
};
