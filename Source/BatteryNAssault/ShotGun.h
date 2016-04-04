// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TraceWeapon.h"
#include "ShotGun.generated.h"

/**
 * 
 */
UCLASS()
class BATTERYNASSAULT_API AShotGun : public ATraceWeapon
{
	GENERATED_BODY()
public:
	AShotGun();

protected:
	UPROPERTY(EditDefaultsOnly)
		float Spread = 8;
	

private:
		void Attack() override;

		

	
	
	
	
};
