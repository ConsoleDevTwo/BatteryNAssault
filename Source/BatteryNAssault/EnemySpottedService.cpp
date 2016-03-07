// Fill out your copyright notice in the Description page of Project Settings.

#include "BatteryNAssault.h"
#include "EnemySpottedService.h"


UEnemySpottedService::UEnemySpottedService()
{
	
}

void UEnemySpottedService::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	ReceiveTick(OwnerComp, DeltaSeconds);
	
}

void UEnemySpottedService::ReceiveTick(UBehaviorTreeComponent & OwnerComp, float DeltaSeconds)
{
	
}

