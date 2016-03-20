// Fill out your copyright notice in the Description page of Project Settings.

#include "BatteryNAssault.h"
#include "PatrolService.h"

// AI module includes
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
// Includes key types
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"

#include "MechAIController.h"
#include "BatteryNAssaultCharacter.h"
#include "MechAICharacter.h"


UPatrolService::UPatrolService()
{

}


void UPatrolService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// If the blackboard comp doesnt exist, exit
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp)
		return;

	// Get the mech controller and see if it exists
	AMechAIController* MechController = Cast<AMechAIController>(OwnerComp.GetAIOwner());
	if (!MechController)
		return;

	// Get the ai character and see if it exists
	AMechAICharacter* AICharacter = Cast<AMechAICharacter>(MechController->GetControlledPawn());
	if (!AICharacter)
		return;

	if (AICharacter->State != AIStates::SEARCHING)
	{
		AICharacter->State = AIStates::SEARCHING;
	}

}