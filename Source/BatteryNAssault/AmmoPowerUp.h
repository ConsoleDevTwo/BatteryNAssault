// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PowerUp.h"
#include "AmmoPowerUp.generated.h"

/**
 * 
 */
UCLASS()
class BATTERYNASSAULT_API AAmmoPowerUp : public APowerUp
{

	GENERATED_BODY()
	
public:
	virtual void ExecutePowerUp(AActor * instigator) override;
	
	
};
