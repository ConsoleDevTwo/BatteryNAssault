// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Weapon.h"
#include "RocketLauncher.generated.h"

/**
 * 
 */
UCLASS()
class BATTERYNASSAULT_API ARocketLauncher : public AWeapon
{
	GENERATED_BODY()


	public:
		ARocketLauncher();


	private:
		void Attack() override;

	protected:
		UPROPERTY(EditAnywhere)
			TSubclassOf<AMyProjectile> ProjectileClass;
		UPROPERTY(VisibleAnywhere)
			UStaticMeshComponent* Mesh;





	};
	
	
	
	

