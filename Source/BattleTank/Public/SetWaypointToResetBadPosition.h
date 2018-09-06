// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "SetWaypointToResetBadPosition.generated.h"

/**
 * 
 */
UCLASS()
class BATTLETANK_API USetWaypointToResetBadPosition : public UBTTaskNode
{
	GENERATED_BODY()
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;


private:
	UPROPERTY(EditAnywhere, Category = "Distance")
	float DistanceFromActor = 1000.f;
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	struct FBlackboardKeySelector WaypointKey;
	
	
};
