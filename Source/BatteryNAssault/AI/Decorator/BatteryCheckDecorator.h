// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BatteryCheckDecorator.generated.h"

/**
 * 
 */
UCLASS()
class BATTERYNASSAULT_API UBatteryCheckDecorator : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()

	UBatteryCheckDecorator(const FObjectInitializer& ObjectInitializer);

	// Describes what we are looking for as a condition
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	EBlackboardNotificationResult OnBlackboardKeyValueChange(const UBlackboardComponent& Blackboard, FBlackboard::FKey ChangedKeyID);

	virtual FString GetStaticDescription() const override;

protected:
	/** called when execution flow controller becomes active */
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	/** called when execution flow controller becomes inactive */
	virtual void OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
public:

	// The upper threshold of when the mech is done charging it's battery
	UPROPERTY(EditAnywhere)
	float UpperThreshhold = 70.0f;

	// The threshold of health
	UPROPERTY(EditAnywhere)
	float Threshhold = 20.0f;
};
