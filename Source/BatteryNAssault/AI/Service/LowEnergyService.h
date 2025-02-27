// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "LowEnergyService.generated.h"

/**
 * 
 */
UCLASS()
class BATTERYNASSAULT_API ULowEnergyService : public UBTService_BlackboardBase
{
	GENERATED_BODY()

	ULowEnergyService();
public:

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
