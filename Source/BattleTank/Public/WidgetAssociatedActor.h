// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "WidgetAssociatedActor.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UWidgetAssociatedActor : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BATTLETANK_API IWidgetAssociatedActor
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Health")
	float GetHealthPercent() const;	

};
