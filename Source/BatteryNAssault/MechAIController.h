// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
/* AI Specific includes */
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MechAICharacter.h"

#include "MechAIController.generated.h"

/**
 * 
 */
UCLASS()
class BATTERYNASSAULT_API AMechAIController : public AAIController
{
	GENERATED_BODY()


	AMechAIController(const class FObjectInitializer& ObjectInitializer);
	
	/* Called whenever the controller possesses a character bot */
	virtual void Possess(class APawn* InPawn) override;

	virtual void UnPossess() override;

	UBehaviorTreeComponent* BehaviorComp;

	UBlackboardComponent* BlackboardComp;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName TargetEnemyKeyName;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName WaypointTarget;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName SightedPosKeyName;

	AMechAICharacter* MyCharacter;

public:
	void SetTargetEnemy(APawn* NewTarget);
	void SetNextWaypoint(AActor* NextWaypoint);

	AActor* GetCurrentWaypoint();
	int8 GetTeamID();
	float GetBatteryCharge();
};
