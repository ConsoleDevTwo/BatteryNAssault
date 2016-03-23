// Fill out your copyright notice in the Description page of Project Settings.

#include "BatteryNAssault.h"
#include "BatteryCheckDecorator.h"

#include "MechAIController.h"
#include "MechAICharacter.h"

// AI module includes
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
// Includes key types
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"

UBatteryCheckDecorator::UBatteryCheckDecorator(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "Battery Check";

	bNotifyBecomeRelevant = true;
	bNotifyCeaseRelevant = true;
}

bool UBatteryCheckDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AMechAIController* MyController = Cast<AMechAIController>(OwnerComp.GetAIOwner());
	if (MyController == nullptr)
	{
		return false;
	}


	AMechAICharacter* AICharacter = Cast<AMechAICharacter>(MyController->GetPawn());
	if (!AICharacter)
	{
		return false;
	}

	if (MyController->GetBatteryCharge() <= Threshhold)
	{
		AICharacter->bIsCharging = true;
		return true;
	}
	else if (AICharacter->bIsCharging)
	{
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::White, "Please");
		if (MyController->GetBatteryCharge() <= UpperThreshhold)
		{
			return true;
		}
		else
		{
			AICharacter->bIsCharging = false;
			return false;
		}
	}
	return false;
}

void UBatteryCheckDecorator::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (BlackboardComp)
	{
		auto KeyID = BlackboardKey.GetSelectedKeyID();
		BlackboardComp->RegisterObserver(KeyID, this, FOnBlackboardChangeNotification::CreateUObject(this, &UBTDecorator_BlackboardBase::OnBlackboardKeyValueChange));
	}
}

void UBatteryCheckDecorator::OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (BlackboardComp)
	{
		BlackboardComp->UnregisterObserversFrom(this);
	}
}

EBlackboardNotificationResult UBatteryCheckDecorator::OnBlackboardKeyValueChange(const UBlackboardComponent& Blackboard, FBlackboard::FKey ChangedKeyID)
{
	check(Cast<UBehaviorTreeComponent>(Blackboard.GetBrainComponent()));
	check(BlackboardKey.GetSelectedKeyID() == ChangedKeyID);

	UBehaviorTreeComponent* BehaviorComp = (UBehaviorTreeComponent*)Blackboard.GetBrainComponent();


	// using nullptr for memory since we're not using any
	if (CalculateRawConditionValue(*CastChecked<UBehaviorTreeComponent>(Blackboard.GetBrainComponent()), nullptr) != IsInversed())
	{
		BehaviorComp->RequestExecution(this);
	}

	return EBlackboardNotificationResult::ContinueObserving;
}

FString UBatteryCheckDecorator::GetStaticDescription() const
{
	return FString::Printf(TEXT("Check if battery is less then %f"), Threshhold);
}