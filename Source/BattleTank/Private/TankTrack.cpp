// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTrack.h"
#include "Engine/World.h"
#include "SprungWheel.h"
#include "SpawnPoint.h"
#include "typeinfo"

UTankTrack::UTankTrack() {
	PrimaryComponentTick.bCanEverTick = true;
}

void UTankTrack::BeginPlay()
{
	Super::BeginPlay();
}


void UTankTrack::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

TArray<ASprungWheel*> UTankTrack::GetWheels() const
{
	TArray<USceneComponent*> spawnPointComponents;
	GetChildrenComponents(false, spawnPointComponents);
	if (!spawnPointComponents.IsValidIndex(0)) return TArray<ASprungWheel*>();
	TArray<ASprungWheel*> sprungWheels = TArray<ASprungWheel*>();
	for (auto component : spawnPointComponents) {
		USpawnPoint * spawnPoint = Cast<USpawnPoint>(component);
		if (!spawnPoint) continue;
		TArray<AActor*> actors = spawnPoint->GetActorsSpawned();
		for (auto actor : actors) {
			ASprungWheel* sprungWheel = Cast<ASprungWheel>(actor);
			if (!sprungWheel) continue;
			sprungWheels.Add(sprungWheel);
		}
	}
	return sprungWheels;
}

void UTankTrack::SetThrottle(float Throttle) {
	float CurrentThrottle = FMath::Clamp<float>(Throttle, -1, 1);
	DriveTrack(CurrentThrottle);
}

void UTankTrack::DriveTrack(float CurrentThrottle)
{
	auto ForceApplied = CurrentThrottle * MaxDrivingForce;
	auto Wheels = GetWheels();
	for (auto wheel : Wheels) {
		wheel->AddDrivingForce(ForceApplied);
	}
}

void UTankTrack::Boost(float Throttle) {
	Throttle = FMath::Clamp<float>(Throttle, -1, 1);
	auto ImpulseApplied = Throttle * MaxDrivingForce;
	auto Wheels = GetWheels();
	for (auto wheel : Wheels) {
		wheel->AddBoost(ImpulseApplied);
	}
}





