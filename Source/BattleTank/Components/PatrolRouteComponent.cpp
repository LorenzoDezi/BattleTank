// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/PatrolRouteComponent.h"


// Sets default values for this component's properties
UPatrolRouteComponent::UPatrolRouteComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

TArray<AActor*> UPatrolRouteComponent::GetPatrolPoints() {
	return PatrolPoints;
}

AActor * UPatrolRouteComponent::GetPatrolPointAt(int32 index)
{
	if (index >= PatrolPoints.Num() || index < 0)
		return nullptr;
	else
		return PatrolPoints[index];
}

int32 UPatrolRouteComponent::GetNumberOfPatrolPoints()
{
	return PatrolPoints.Num();
}

void UPatrolRouteComponent::SetPatrolPoints(TArray<AActor*> patrolPoints)
{
	PatrolPoints = patrolPoints;
}


