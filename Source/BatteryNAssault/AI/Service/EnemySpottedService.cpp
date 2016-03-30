// Fill out your copyright notice in the Description page of Project Settings.

#include "BatteryNAssault.h"
#include "EnemySpottedService.h"

// AI module includes
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
// Includes key types
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"

#include "MechAIController.h"
#include "BatteryNAssaultCharacter.h"
#include "MechAICharacter.h"


UEnemySpottedService::UEnemySpottedService()
{
	
}

void UEnemySpottedService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
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


	// Get the enemy and see if it exists
	ABatteryNAssaultCharacter* Target = Cast<ABatteryNAssaultCharacter>(BlackboardComp->GetValueAsObject(GetSelectedBlackboardKey()));
	if (!Target)
	{
		if (AICharacter->State != AIStates::SEARCHING)
		{
			AICharacter->State = AIStates::SEARCHING;
		}
		return;
	}


	if (AICharacter->State != AIStates::SPOTTED)
	{
		AICharacter->State = AIStates::SPOTTED;
	}


	AICharacter->LookLocation =  Target->GetActorLocation() + Target->GetVelocity() * DeltaSeconds;

	
}
