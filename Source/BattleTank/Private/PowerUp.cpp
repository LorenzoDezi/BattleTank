// Fill out your copyright notice in the Description page of Project Settings.

#include "PowerUp.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Engine/World.h"


// Sets default values
APowerUp::APowerUp()
{
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APowerUp::BeginPlay()
{
	Super::BeginPlay();
	UStaticMeshComponent* mesh = FindComponentByClass<UStaticMeshComponent>();
	if(mesh)
		mesh->OnComponentHit.AddDynamic(this, &APowerUp::OnHit);
}

// Called every frame
void APowerUp::Tick(float DeltaTime)
{
	FRotator rotation = this->GetActorRotation();
	this->SetActorRotation(FRotator(
		rotation.Pitch,
		rotation.Yaw + DeltaTime * RotationDegreePerSecond,
		rotation.Roll 
	));
	if (!GetEnabledPercent())
		enabled = true;
}

void APowerUp::OnHit(UPrimitiveComponent* HitComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	FVector NormalImpulse,
	const FHitResult& Hit)
{
	if (!enabled) return;
	LastTimeHit = GetWorld()->GetTimeSeconds();
	enabled = false;
}

float APowerUp::GetEnabledPercent()
{
	float result =  (float) ((LastTimeHit + TimeToEnable) - GetWorld()->GetTimeSeconds()) / (float) TimeToEnable;
	if (result < 0.f) result = 0.f;
	return result;
}

bool APowerUp::IsEnabled()
{
	return enabled;
}

