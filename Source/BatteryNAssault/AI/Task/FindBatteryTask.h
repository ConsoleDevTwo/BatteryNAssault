// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "FindBatteryTask.generated.h"

/**
 * 
 */
UCLASS()
class BATTERYNASSAULT_API UFindBatteryTask : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
public:
	UPROPERTY(EditAnywhere)
	bool bIsSameTeam = false;

	UPROPERTY(EditAnywhere)
	float SearchRadius = 200.0f;
};
