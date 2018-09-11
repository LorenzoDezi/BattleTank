// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/SpawnPoint.h"
#include "Engine/World.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"


USpawnPoint::USpawnPoint()
{
	PrimaryComponentTick.bCanEverTick = false;
	
}

TArray<AActor*> USpawnPoint::GetActorsSpawned()
{
	return ActorsSpawned;
}

void USpawnPoint::BeginPlay()
{
	if (!ActorToSpawn) return;
	AActor* actorSpawned = GetWorld()->SpawnActorDeferred<AActor>(ActorToSpawn, FTransform(FVector(0,0,240)));
	actorSpawned->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
	UGameplayStatics::FinishSpawningActor(actorSpawned, FTransform(FVector(0, 0, 240)));
	ActorsSpawned.Add(actorSpawned);
	Super::BeginPlay();
}
