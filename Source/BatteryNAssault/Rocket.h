// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MyProjectile.h"
#include "Rocket.generated.h"

/**
 * 
 */
UCLASS()
class BATTERYNASSAULT_API ARocket : public AMyProjectile
{
	GENERATED_BODY()
public:

	virtual void Tick(float DeltaSeconds) override;






protected:
	


		void OnBeginOverlap(AActor* OtherActor) override;

		float Acceleration;
		float Speed = 1000.0f;
	
	
	
};
