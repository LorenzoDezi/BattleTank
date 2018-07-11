// Fill out your copyright notice in the Description page of Project Settings.

#include "TankMovementComponent.h"
#include "Engine/World.h"
#include "TankTrack.h"



void UTankMovementComponent::IntendMoveForward(float Throw) {
	if(ensure(LeftTrack))
		LeftTrack->SetThrottle(Throw);
	if(ensure(RightTrack))
		RightTrack->SetThrottle(Throw);
}

void UTankMovementComponent::IntendTurnRight(float Throw) {
	if(ensure(RightTrack))
		RightTrack->SetThrottle(Throw);
}

void UTankMovementComponent::IntendTurnLeft(float Throw) {
	if(ensure(LeftTrack))	
		LeftTrack->SetThrottle(Throw);
}

void UTankMovementComponent::RequestDirectMove(const FVector & MoveVelocity, bool bForceMaxSpeed)
{
	auto CurrentDirection = GetOwner()->GetActorForwardVector().GetSafeNormal();
	auto DirectionToAssume = MoveVelocity.GetSafeNormal();

	auto ForwardThrow = FVector::DotProduct(CurrentDirection, DirectionToAssume);
	auto RightThrow = FVector::CrossProduct(CurrentDirection, DirectionToAssume).Z;
	auto LeftThrow = FVector::CrossProduct(DirectionToAssume, CurrentDirection).Z;

	IntendMoveForward(ForwardThrow);
	IntendTurnLeft(LeftThrow);
	IntendTurnRight(RightThrow);
}

void UTankMovementComponent::Initialise(UTankTrack * leftTrack, UTankTrack * rightTrack)
{
	LeftTrack = leftTrack;
	RightTrack = rightTrack;
}
