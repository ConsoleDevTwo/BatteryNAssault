// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "FindCombatLocationTask.generated.h"

/**
 * 
 */
UCLASS()
class BATTERYNASSAULT_API UFindCombatLocationTask : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:
	UPROPERTY(EditAnywhere)
		float LocationRadius = 1000.0f;
	
	
};
