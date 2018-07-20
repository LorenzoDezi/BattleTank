// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTrack.h"
#include "Engine/World.h"
#include "SprungWheel.h"

UTankTrack::UTankTrack() {
	PrimaryComponentTick.bCanEverTick = true;
}

void UTankTrack::BeginPlay()
{
	Super::BeginPlay();
	OnComponentHit.AddDynamic(this, &UTankTrack::OnHit);
}


void UTankTrack::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UTankTrack::ApplySidewaysFriction()
{
	float DeltaTime = GetWorld()->GetTimeSeconds();
	auto slippageSpeed = FVector::DotProduct(GetComponentVelocity(), GetRightVector());
	auto correctAcceleration = -slippageSpeed / DeltaTime * GetRightVector();
	auto TankRootComponent = Cast<UStaticMeshComponent>(GetOwner()->GetRootComponent());
	auto correctionForce = (TankRootComponent->GetMass() * correctAcceleration) / 2;
	TankRootComponent->AddForce(correctionForce);
}

void UTankTrack::OnHit(UPrimitiveComponent * HitComponent, AActor * OtherActor, UPrimitiveComponent * OtherComponent, FVector NormalImpulse, const FHitResult & Hit)
{
	DriveTrack();
	ApplySidewaysFriction();
	CurrentThrottle = 0;
}


void UTankTrack::SetThrottle(float Throttle) {
	CurrentThrottle += Throttle;
	CurrentThrottle = FMath::Clamp<float>(CurrentThrottle, -1, 1);

}

void UTankTrack::DriveTrack()
{
	auto ComponentLocation = GetComponentLocation();
	auto TankRootComponent = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
	auto velocity = TankRootComponent->GetComponentVelocity();
	TankRootComponent->AddForceAtLocation(GetForwardVector() * CurrentThrottle * MaxDrivingForce, ComponentLocation);
}



