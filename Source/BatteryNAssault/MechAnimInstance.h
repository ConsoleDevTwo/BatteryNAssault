// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimInstance.h"
#include "MechAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class BATTERYNASSAULT_API UMechAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeUpdateAnimation(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FRotator TowerRotation;

	
	
	
};
