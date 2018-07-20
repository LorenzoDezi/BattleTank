// Fill out your copyright notice in the Description page of Project Settings.

#include "SprungWheel.h"
#include "Runtime/Engine/Classes/Components/SceneComponent.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/PhysicsEngine/PhysicsConstraintComponent.h"
#include "Runtime/Engine/Classes/Components/SphereComponent.h"


// Sets default values
ASprungWheel::ASprungWheel()
{
	PrimaryActorTick.bCanEverTick = false;

	Spring = CreateAbstractDefaultSubobject<UPhysicsConstraintComponent>(FName("Spring"));
	SetRootComponent(Spring);

	Axis = CreateAbstractDefaultSubobject<USphereComponent>(FName("Axis"));
	Axis->AttachToComponent(Spring, FAttachmentTransformRules::KeepRelativeTransform);

	Wheel = CreateAbstractDefaultSubobject<USphereComponent>(FName("Wheel"));
	Wheel->AttachToComponent(Axis, FAttachmentTransformRules::KeepRelativeTransform);
	

	WheelAxisConstraint = CreateAbstractDefaultSubobject<UPhysicsConstraintComponent>(FName("WheelAxisConstraint"));
	WheelAxisConstraint->AttachToComponent(Axis, FAttachmentTransformRules::KeepRelativeTransform);
	


}

// Called when the game starts or when spawned
void ASprungWheel::BeginPlay()
{
	Super::BeginPlay();
	if (!GetAttachParentActor()) return;
	Spring->SetConstrainedComponents(
		Cast<UPrimitiveComponent>(Wheel),
		NAME_None,
		Cast<UPrimitiveComponent>(GetAttachParentActor()->GetRootComponent()),
		NAME_None
	);
	
}

