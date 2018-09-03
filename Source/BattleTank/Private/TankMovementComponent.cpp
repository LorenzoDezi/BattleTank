// Fill out your copyright notice in the Description page of Project Settings.

#include "TankMovementComponent.h"
#include "Engine/World.h"
#include "TankTrack.h"


UTankMovementComponent::UTankMovementComponent() {
	PrimaryComponentTick.bCanEverTick = true;
}

void UTankMovementComponent::IntendMoveForward(float Throw) {
	if(LeftTrack)
		LeftTrack->SetThrottle(Throw);
	if(RightTrack)
		RightTrack->SetThrottle(Throw);
}

void UTankMovementComponent::IntendTurnRight(float Throw) {
	if(RightTrack)
		RightTrack->SetThrottle(Throw);
	if (LeftTrack)
		LeftTrack->SetThrottle(-Throw/2);
}

void UTankMovementComponent::IntendTurnLeft(float Throw) {
	if(LeftTrack)	
		LeftTrack->SetThrottle(Throw);
	if (RightTrack)
		RightTrack->SetThrottle(-Throw/2);
}

void UTankMovementComponent::RequestDirectMove(const FVector & MoveVelocity, bool bForceMaxSpeed)
{
	
	auto CurrentDirection = GetOwner()->GetActorForwardVector().GetSafeNormal();
	auto DirectionToAssume = MoveVelocity.GetSafeNormal();

	auto ForwardThrow = FVector::DotProduct(CurrentDirection, DirectionToAssume);
	auto RightThrow = FVector::CrossProduct(CurrentDirection, DirectionToAssume).Z;
	auto LeftThrow = FVector::CrossProduct(DirectionToAssume, CurrentDirection).Z;
	UE_LOG(LogTemp, Warning, TEXT("Forward: %f, Right: %f, Left: %f"), ForwardThrow, RightThrow, LeftThrow);
	IntendMoveForward(ForwardThrow);
	if(LeftThrow > RightThrow)
		IntendTurnLeft(LeftThrow);
	else
		IntendTurnRight(RightThrow);
}

void UTankMovementComponent::Initialise(UTankTrack * leftTrack, UTankTrack * rightTrack)
{
	LeftTrack = leftTrack;
	RightTrack = rightTrack;
}

void UTankMovementComponent::Boost(float Throttle) {
	//TODO: Play "Empty" sound?
	if (NumberOfBoosts < 1) return;

	LeftTrack->Boost(Throttle);
	RightTrack->Boost(Throttle);
	NumberOfBoosts--;
	LastTimeUsedBoost = GetWorld()->GetTimeSeconds();
}

int32 UTankMovementComponent::GetCurrentBoosts() const
{
	return NumberOfBoosts;
}

void UTankMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	NumberOfBoosts = MaxNumberOfBoosts;
}

void UTankMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	//TODO: Check GetTimeSeconds
	if (GetWorld()->GetTimeSeconds() > LastTimeUsedBoost + TimeToRecoverBoosts
		&& NumberOfBoosts < MaxNumberOfBoosts) {
		NumberOfBoosts = MaxNumberOfBoosts;
	}
}

