// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Actors/Controllers/MachineAIController.h"
#include "SetTankState.generated.h"

/**
 * 
 */
UCLASS()
class BATTLETANK_API USetTankState : public UBTTaskNode
{
	GENERATED_BODY()
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere, Category = "State")
	EMachineAIState StateToSet;
	
};
