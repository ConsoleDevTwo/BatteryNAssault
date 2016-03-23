// Fill out your copyright notice in the Description page of Project Settings.

#include "BatteryNAssault.h"
#include "FindBatteryTask.h"

#include "MechAIController.h"
#include "BaseBattery.h"

// AI module includes
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
// Includes key types
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"

EBTNodeResult::Type UFindBatteryTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	AMechAIController* MyController = Cast<AMechAIController>(OwnerComp.GetAIOwner());
	if (MyController == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	TArray<AActor*> Batteries;
	
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseBattery::StaticClass(), Batteries);

	if (Batteries.Num() <= 0)
	{
		return EBTNodeResult::Failed;
	}

	for (int i = 0; i < Batteries.Num(); i++)
	{
		AActor* Battery = Batteries[i];
		if ((Cast<ABaseBattery>(Battery)->TeamID == MyController->GetTeamID()) == bIsSameTeam)
		{
			/* Find a position that is close to the waypoint. We add a small random to this position to give build predictable patrol patterns  */
			const FVector SearchOrigin = Battery->GetActorLocation();
			const FVector Loc = UNavigationSystem::GetRandomPointInNavigableRadius(MyController, SearchOrigin, SearchRadius);

			/* The selected key should be "TargetLocation" in the BehaviorTree setup */
			OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Vector>(BlackboardKey.GetSelectedKeyID(), Loc);
			return EBTNodeResult::Succeeded;
		}
	}




	return EBTNodeResult::Failed;
}
