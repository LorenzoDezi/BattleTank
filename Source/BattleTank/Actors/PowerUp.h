// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PowerUp.generated.h"

class AMachine;

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
	//PowerUps the machine.
	virtual void PowerUp(AMachine* machine);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	//Function called when the power up collides with another actor.
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult & SweepResult);
	//Returns the time remaining to enable the powerup, 0.0 means loaded.
	UFUNCTION(BlueprintCallable, Category = "PowerUp")
	float GetEnabledPercent();
	UFUNCTION(BlueprintCallable, Category = "PowerUp")
	bool IsEnabled();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float RotationDegreePerSecond = 5.f;
	float LastTimeHit = 0.f;
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float TimeToEnable = 10.f;
	UPROPERTY(EditDefaultsOnly, Category = "Appearance")
	UMaterialInterface* disabledMaterial = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = "Appearance")
	UMaterialInterface* enabledMaterial = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundBase* SoundWhenPicked = nullptr;
	bool enabled = true;
	void SetMaterial(UMaterialInterface* material);
	
};
