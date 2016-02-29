// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Weapon.h"
#include "TraceWeapon.generated.h"

/**
 * 
 */
UCLASS()
class BATTERYNASSAULT_API ATraceWeapon : public AWeapon
{
	GENERATED_BODY()
public:
	ATraceWeapon();


private:
	virtual void Attack() override;
protected:

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere)
		float DamageDealt;
	UPROPERTY(EditDefaultsOnly)
		float Range;

	FHitResult WeaponTrace(FVector &TraceFrom, FVector &TraceTo);

	virtual void HitResult(FHitResult Impact, const FVector Origin, FVector ShootDir);
	
	
	
	
};
