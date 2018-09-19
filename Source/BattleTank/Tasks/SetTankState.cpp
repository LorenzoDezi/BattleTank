// Fill out your copyright notice in the Description page of Project Settings.

#include "SetTankState.h"
#include "Engine/World.h"

EBTNodeResult::Type USetTankState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	auto tankControllerAI = Cast<AMachineAIController>(OwnerComp.GetAIOwner());
	tankControllerAI->SetState(StateToSet);
	return EBTNodeResult::Succeeded;
}




