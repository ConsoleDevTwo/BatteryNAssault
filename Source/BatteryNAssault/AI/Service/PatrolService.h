// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "PatrolService.generated.h"

/**
 * 
 */
UCLASS()
class BATTERYNASSAULT_API UPatrolService : public UBTService_BlackboardBase
{
	GENERATED_BODY()

	UPatrolService();

public:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
	
};
