// Fill out your copyright notice in the Description page of Project Settings.

#include "SprungWheel.h"
#include "Engine/World.h"
#include "Runtime/Engine/Classes/Components/SceneComponent.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/PhysicsEngine/PhysicsConstraintComponent.h"
#include "Runtime/Engine/Classes/Components/SphereComponent.h"


// Sets default values
ASprungWheel::ASprungWheel()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PostPhysics;

	Spring = CreateAbstractDefaultSubobject<UPhysicsConstraintComponent>(FName("Spring"));
	SetRootComponent(Spring);

	Axis = CreateAbstractDefaultSubobject<USphereComponent>(FName("Axis"));
	Axis->SetupAttachment(Spring);

	Wheel = CreateAbstractDefaultSubobject<USphereComponent>(FName("Wheel"));
	Wheel->SetupAttachment(Axis);
	

	WheelAxisConstraint = CreateAbstractDefaultSubobject<UPhysicsConstraintComponent>(FName("WheelAxisConstraint"));
	WheelAxisConstraint->SetupAttachment(Axis);
}

void ASprungWheel::ApplyForce()
{
	UE_LOG(LogTemp, Warning, TEXT("Force applied on wheel %f"), CurrentForceToApply);
	Wheel->AddForce(Axis->GetForwardVector() * CurrentForceToApply);
}

void ASprungWheel::AddBoost(float ForceMagnitude)
{
	Wheel->AddImpulse(Axis->GetForwardVector() * ForceMagnitude);
}

// Called when the game starts or when spawned
void ASprungWheel::BeginPlay()
{
	Super::BeginPlay();
	Wheel->SetNotifyRigidBodyCollision(true);
	Wheel->OnComponentHit.AddDynamic(this, &ASprungWheel::OnHit);
	if (!GetAttachParentActor()) return;
	Spring->SetConstrainedComponents(
		Cast<UPrimitiveComponent>(GetAttachParentActor()->FindComponentByClass<UStaticMeshComponent>()),
		NAME_None,
		Cast<UPrimitiveComponent>(Axis),
		NAME_None
	);
	WheelAxisConstraint->SetConstrainedComponents(
		Cast<UPrimitiveComponent>(Axis),
		NAME_None,
		Cast<UPrimitiveComponent>(Wheel),
		NAME_None
	);
	
}

void ASprungWheel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (GetWorld()->TickGroup == TG_PostPhysics) {
		CurrentForceToApply = 0.f;
	}
}

void ASprungWheel::AddDrivingForce(float ForceMagnitude)
{
	if(!CurrentForceToApply)
		CurrentForceToApply = ForceMagnitude;
}

void ASprungWheel::OnHit(UPrimitiveComponent * HitComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, FVector NormalImpulse, const FHitResult & Hit)
{
	ApplyForce();
}

