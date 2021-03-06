// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/TankTrack.h"
#include "Engine/World.h"
#include "Actors/SprungWheel.h"
#include "Components/SpawnPoint.h"
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
		if (component && component->IsA<USpawnPoint>()) {
			USpawnPoint * spawnPoint = Cast<USpawnPoint>(component);
			TArray<AActor*> actors = spawnPoint->GetActorsSpawned();
			for (auto actor : actors) {
				if (actor && actor->IsA<ASprungWheel>()) {
					ASprungWheel* sprungWheel = Cast<ASprungWheel>(actor);
					sprungWheels.Add(sprungWheel);
				}
			}
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





