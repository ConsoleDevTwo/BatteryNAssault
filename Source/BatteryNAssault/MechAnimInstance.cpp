// Fill out your copyright notice in the Description page of Project Settings.

#include "BatteryNAssault.h"
#include "MechAnimInstance.h"
#include "BatteryNAssaultCharacter.h"


void UMechAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	APawn* OwningCharacter = TryGetPawnOwner();
	if (OwningCharacter)
	{
		ABatteryNAssaultCharacter* MyCharacter = Cast<ABatteryNAssaultCharacter>(OwningCharacter);
		if (MyCharacter)
		{
			TowerRotationY = MyCharacter->TowerRotationY;
			TowerRotationZ = MyCharacter->TowerRotationZ;
			TowerRotation = MyCharacter->TowerRotation;
		}
	}
}


