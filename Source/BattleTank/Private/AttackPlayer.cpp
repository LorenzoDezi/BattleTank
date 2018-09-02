// Fill out your copyright notice in the Description page of Project Settings.

#include "AttackPlayer.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "Tank.h"
#include "BehaviorTree/BlackboardComponent.h"


EBTNodeResult::Type UAttackPlayer::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	auto BlackboardComponent = OwnerComp.GetBlackboardComponent();
	auto TankPlayer = BlackboardComponent->GetValueAsObject(TankPlayerKey.SelectedKeyName);
	auto tankAsPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (tankAsPawn == nullptr) return EBTNodeResult::Failed;
	auto tank = Cast<ATank>(tankAsPawn);
	tank->AimAt(tank->GetActorLocation());
	return EBTNodeResult::Succeeded;
}
