// Fill out your copyright notice in the Description page of Project Settings.

#include "BatteryNAssault.h"
#include "FIndPatrolLocationTask.h"

#include "MechAIController.h"

// AI module includes
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
// Includes key types
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"

EBTNodeResult::Type UFIndPatrolLocationTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	AMechAIController* MyController = Cast<AMechAIController>(OwnerComp.GetAIOwner());
	if (MyController == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	AActor* MyWaypoint = MyController->GetCurrentWaypoint();
	if (MyWaypoint)
	{
		/* Find a position that is close to the waypoint. We add a small random to this position to give build predictable patrol patterns  */
		const FVector SearchOrigin = MyWaypoint->GetActorLocation();
		const FVector Loc = UNavigationSystem::GetRandomPointInNavigableRadius(MyController, SearchOrigin, SearchRadius);
		// If the location isnt a zero vector, set the target location
		if (Loc != FVector::ZeroVector)
		{
			/* The selected key should be "PatrolLocation" in the BehaviorTree setup */
			OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Vector>(BlackboardKey.GetSelectedKeyID(), Loc);
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}