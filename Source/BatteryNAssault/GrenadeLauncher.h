// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Weapon.h"
#include "Grenade.h"
#include "GrenadeLauncher.generated.h"

/**
 * 
 */
UCLASS()
class BATTERYNASSAULT_API AGrenadeLauncher : public AWeapon
{
	GENERATED_BODY()

public:
	AGrenadeLauncher();
	void EndAttack()override;
	UPROPERTY(EditDefaultsOnly)
		float GrenadeSpeed;

	virtual void Tick(float DeltaSeconds) override;

private:
	void Attack()override;


protected:
	UPROPERTY(EditAnywhere)
		TSubclassOf<AGrenade> ProjectileClass;
	UPROPERTY(VisibleAnywhere)
		USkeletalMeshComponent* Mesh;
	UPROPERTY(EditDefaultsOnly)
		float TimeBetweenShots;

	float TimeLeft;
	
	
	
	
};
