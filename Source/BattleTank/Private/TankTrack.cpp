// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTrack.h"
#include "Engine/World.h"

void UTankTrack::SetThrottle(float Throttle) {

	//TODO Throttling
	UE_LOG(LogTemp, Warning, TEXT("%f: Throttle at %f"), GetWorld()->GetTimeSeconds(), Throttle);

	auto ComponentLocation = GetComponentLocation();
	auto TankRootComponent = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
	TankRootComponent->AddForceAtLocation(GetForwardVector() * Throttle * MaxDrivingForce, ComponentLocation);
}



