// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MyProjectile.h"
#include "Grenade.generated.h"

/**
 * 
 */
UCLASS()
class BATTERYNASSAULT_API AGrenade : public AMyProjectile
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ParticleSystem)
		UParticleSystem* flame2;

protected:
	void OnBeginOverlap(AActor* OtherActor) override;

	void OnConstruction(const FTransform & Transform) override;
};
