// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Weapon.h"
#include "MachineGun.generated.h"

/**
 * 
 */
UCLASS()
class BATTERYNASSAULT_API AMachineGun : public AWeapon
{
	GENERATED_BODY()

public:
	AMachineGun();


private:
	void Attack() override;

protected:
	UPROPERTY(EditAnywhere)
		TSubclassOf<AMyProjectile> ProjectileClass;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio Component")
	class UAudioComponent* AudioComp;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		USoundCue* FiringSound;
	
	
	
	
};
