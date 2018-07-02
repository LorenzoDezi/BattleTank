// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTrack.h"
#include "Engine/World.h"

void UTankTrack::SetThrottle(float Throttle) {

	//TODO Clamp the throttle value so that no one can double the input
	auto ComponentLocation = GetComponentLocation();
	auto TankRootComponent = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
	TankRootComponent->AddForceAtLocation(GetForwardVector() * Throttle * MaxDrivingForce, ComponentLocation);
}



