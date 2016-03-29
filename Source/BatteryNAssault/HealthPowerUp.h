// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PowerUp.h"
#include "HealthPowerUp.generated.h"

/**
 * 
 */
UCLASS()
class BATTERYNASSAULT_API AHealthPowerUp : public APowerUp
{
	GENERATED_BODY()
	
public:
	virtual void ExecutePowerUp(AActor * instigator) override;
	
	
};
