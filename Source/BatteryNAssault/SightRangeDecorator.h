// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "SightRangeDecorator.generated.h"

/**
 * 
 */
UCLASS()
class BATTERYNASSAULT_API USightRangeDecorator : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()

	USightRangeDecorator(const FObjectInitializer& ObjectInitializer);
public:

	// Describes what we are looking for as a condition
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	UPROPERTY(EditAnywhere)
	float Range;

//	UPROPERTY(EditAnywhere)
//	FBlackboard::FKey EnemyKey;
};
