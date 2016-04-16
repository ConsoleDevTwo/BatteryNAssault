// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "EnemySpottedService.generated.h"

/**
 * 
 */
UCLASS()
class BATTERYNASSAULT_API UEnemySpottedService : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UEnemySpottedService();

	UPROPERTY(EditDefaultsOnly, Category = "Blackboard")
	FName StunTarget = "Stunned";

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
