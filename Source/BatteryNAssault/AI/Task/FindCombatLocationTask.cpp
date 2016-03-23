// Fill out your copyright notice in the Description page of Project Settings.

#include "BatteryNAssault.h"
#include "FindCombatLocationTask.h"

#include "BatteryNAssaultCharacter.h"
#include "MechAIController.h"

// AI module includes
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
// Includes key types
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"

EBTNodeResult::Type UFindCombatLocationTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AMechAIController* MyController = Cast<AMechAIController>(OwnerComp.GetAIOwner());
	if (MyController == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	/* Find a position that is close to the waypoint. We add a small random to this position to give build predictable patrol patterns  */
	ABatteryNAssaultCharacter* Target = Cast<ABatteryNAssaultCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("Enemy"));
	if (!Target)
	{
		return EBTNodeResult::Failed;
	}

	FVector SearchOrigin = (Target->GetActorLocation() + MyController->GetCharacter()->GetActorLocation()) / 2;

	const FVector Loc = UNavigationSystem::GetRandomPointInNavigableRadius(MyController, SearchOrigin, LocationRadius);
	// If the location isnt a zero vector, set the target location
	if (Loc != FVector::ZeroVector)
	{
		/* The selected key should be "PatrolLocation" in the BehaviorTree setup */

		OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Vector>(BlackboardKey.GetSelectedKeyID(), Loc);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
