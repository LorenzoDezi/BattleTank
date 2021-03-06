// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PatrolRouteComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BATTLETANK_API UPatrolRouteComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPatrolRouteComponent();

public:
	TArray<AActor*> GetPatrolPoints();
	AActor* GetPatrolPointAt(int32 index);
	int32 GetNumberOfPatrolPoints();
	void SetPatrolPoints(TArray<AActor*> patrolPoints);

private:
	TArray<AActor*> PatrolPoints = TArray<AActor*>();
};
