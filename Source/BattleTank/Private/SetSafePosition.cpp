// Fill out your copyright notice in the Description page of Project Settings.
#include "SetSafePosition.h"
#include "Engine/World.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "MachineAIController.h"

EBTNodeResult::Type USetSafePosition::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	auto BlackboardComponent = OwnerComp.GetBlackboardComponent();
	auto tankControllerAI = Cast<AMachineAIController>(OwnerComp.GetAIOwner());
	auto pawn = tankControllerAI->GetPawn();
	if (pawn == nullptr) return EBTNodeResult::Failed;
	UNavigationSystemV1* Nav = UNavigationSystemV1::GetNavigationSystem(tankControllerAI->GetWorld());
	FVector RandomLocation;
	Nav->K2_GetRandomReachablePointInRadius(tankControllerAI->GetWorld(), pawn->GetActorLocation(), RandomLocation, DistanceFromActor);
	UE_LOG(LogTemp, Warning, TEXT("RANDOM WAYPOINT %s"), *(RandomLocation.ToString()));
	BlackboardComponent->SetValueAsVector(WaypointKey.SelectedKeyName,RandomLocation);
	return EBTNodeResult::Succeeded;
}
