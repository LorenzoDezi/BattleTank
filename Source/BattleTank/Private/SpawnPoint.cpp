// Fill out your copyright notice in the Description page of Project Settings.

#include "SpawnPoint.h"
#include "Engine/World.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"


USpawnPoint::USpawnPoint()
{
	PrimaryComponentTick.bCanEverTick = false;
	
}

void USpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	if (!ActorToSpawn) return;
	AActor* actorSpawned = GetWorld()->SpawnActorDeferred<AActor>(ActorToSpawn, GetComponentTransform());
	actorSpawned->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
	UE_LOG(LogTemp, Warning, TEXT("Finishing spawning actor"));
	UGameplayStatics::FinishSpawningActor(actorSpawned, GetComponentTransform());
}
