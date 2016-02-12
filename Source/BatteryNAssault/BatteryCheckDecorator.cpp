// Fill out your copyright notice in the Description page of Project Settings.

#include "BatteryNAssault.h"
#include "BatteryCheckDecorator.h"

#include "MechAIController.h"

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

	if (MyController->GetBatteryCharge() <= Threshhold)
	{
		FString Message = FString::Printf(TEXT("Energy: %.2f :  %.2f"), MyController->GetBatteryCharge(), Threshhold);
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::White, Message);
		return true;
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