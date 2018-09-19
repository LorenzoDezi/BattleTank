// Fill out your copyright notice in the Description page of Project Settings.

#include "PatrolCycleWaypoint.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "Components/PatrolRouteComponent.h"
#include "Actors/Controllers/MachineAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UPatrolCycleWaypoint::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	//Setup the components we need
	auto BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComponent) return EBTNodeResult::Failed;
	auto Index = BlackboardComponent->GetValueAsInt(WaypointIndexKey.SelectedKeyName);
	auto tankControllerAI = Cast<AMachineAIController>(OwnerComp.GetAIOwner());
	auto pawn = tankControllerAI->GetPawn();
	if (!pawn) return EBTNodeResult::Failed;
	auto patrolRouteComponent = pawn->FindComponentByClass<UPatrolRouteComponent>();
	if (!patrolRouteComponent) return EBTNodeResult::Failed;
	
	//Setting next waypoint on blackboard
	auto nextWaypoint = patrolRouteComponent->GetPatrolPointAt(Index);
	if (!nextWaypoint) return EBTNodeResult::Failed;
	BlackboardComponent->SetValueAsObject(WaypointKey.SelectedKeyName, nextWaypoint);

	//Cycle the waypoint index
	Index++;
	BlackboardComponent->SetValueAsInt(WaypointIndexKey.SelectedKeyName, Index % (patrolRouteComponent->GetNumberOfPatrolPoints()));
	return EBTNodeResult::Succeeded;
}
