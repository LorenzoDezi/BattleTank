// Fill out your copyright notice in the Description page of Project Settings.

#include "AttackPlayer.h"
#include "Actors/Controllers/MachineAIController.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "Actors/Machine.h"
#include "BehaviorTree/BlackboardComponent.h"


EBTNodeResult::Type UAttackPlayer::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	auto BlackboardComponent = OwnerComp.GetBlackboardComponent();
	auto EnemyAsActor = Cast<AActor>(BlackboardComponent->GetValueAsObject(TankPlayerKey.SelectedKeyName));
	auto TankAI = Cast<AMachineAIController>(OwnerComp.GetAIOwner());
	if (TankAI == nullptr) return EBTNodeResult::Failed;
	TankAI->SetEnemy(EnemyAsActor);
	TankAI->SetState(EMachineAIState::Attacking);
	return EBTNodeResult::Succeeded;
}
