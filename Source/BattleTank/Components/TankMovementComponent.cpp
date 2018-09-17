// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/TankMovementComponent.h"
#include "Engine/World.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Components/TankTrack.h"


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
	auto owner = GetOwner();
	if (!owner) return;
	auto TankMesh = owner->FindComponentByClass<UStaticMeshComponent>();
	if (!TankMesh) return;
	auto CurrentDirection = TankMesh->GetForwardVector().GetSafeNormal();
	auto DirectionToAssume = MoveVelocity.GetSafeNormal();
	
	auto ForwardThrow = FVector::DotProduct(CurrentDirection, DirectionToAssume);
	auto RightThrow = FVector::CrossProduct(CurrentDirection, DirectionToAssume).Z;
	FHitResult OutHit;
	//Refactor TODO
	if (GetWorld()->LineTraceSingleByChannel(OutHit, GetActorLocation(),
		GetActorLocation() + CurrentDirection * HitDangerDistance,
		ECollisionChannel::ECC_Visibility) && OutHit.GetActor()) {
		if (RightThrow > 0)
			RightThrow += SteerFactor / OutHit.Distance;
		else
			RightThrow -= SteerFactor / OutHit.Distance;
	}
	if (FMath::Abs(ForwardThrow) > FMath::Abs(RightThrow)) {
		IntendMoveForward(ForwardThrow);
		RightThrow /= 2;
	}
	if (RightThrow > 0)
		IntendTurnRight(RightThrow);
	else
		IntendTurnLeft(-RightThrow);
}

void UTankMovementComponent::Initialise(UTankTrack * leftTrack, UTankTrack * rightTrack)
{
	LeftTrack = leftTrack;
	RightTrack = rightTrack;
}

void UTankMovementComponent::Boost(float Throttle) {
	
	if (NumberOfBoosts < 1) {
		if(TankBoostEmpty && LeftTrack) 
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), TankBoostEmpty,
			LeftTrack->GetComponentLocation());
		return;
	}

	LeftTrack->Boost(Throttle);
	RightTrack->Boost(Throttle);
	NumberOfBoosts--;
	if (TankBoost && LeftTrack)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), TankBoost,
			LeftTrack->GetComponentLocation());
	LastTimeUsedBoost = GetWorld()->GetTimeSeconds();
}

float UTankMovementComponent::GetBoostPercent() const
{
	return (float) NumberOfBoosts/(float) MaxNumberOfBoosts;
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

