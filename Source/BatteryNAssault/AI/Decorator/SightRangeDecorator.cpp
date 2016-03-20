// Fill out your copyright notice in the Description page of Project Settings.

#include "BatteryNAssault.h"
#include "SightRangeDecorator.h"

#include "MechAIController.h"

// AI module includes
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
// Includes key types
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"

USightRangeDecorator::USightRangeDecorator(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "Sight Range";

	Range = 1000.0f;

	bNotifyBecomeRelevant = true;
	bNotifyCeaseRelevant = true;
}

bool USightRangeDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

	if (!BlackboardComp)
	{
		return false;
	}

	AMechAIController* MyController = Cast<AMechAIController>(OwnerComp.GetAIOwner());
	if (MyController == nullptr)
	{
		return false;
	}

	FVector SightPosition = BlackboardComp->GetValueAsVector(BlackboardKey.GetSelectedKeyID());
	
	//if the player is out of the range to chase then return false
	if (FVector::Dist(SightPosition, MyController->GetCharacter()->GetActorLocation()) > Range)
	{
		BlackboardComp->ClearValue("Enemy");
		return false;
	}

	return true;
}


