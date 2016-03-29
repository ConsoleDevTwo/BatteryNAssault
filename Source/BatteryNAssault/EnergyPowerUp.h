// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PowerUp.h"
#include "EnergyPowerUp.generated.h"

/**
 * 
 */
UCLASS()
class BATTERYNASSAULT_API AEnergyPowerUp : public APowerUp
{
	GENERATED_BODY()
	
public:
	virtual void ExecutePowerUp(AActor * instigator) override;
	
	
};
