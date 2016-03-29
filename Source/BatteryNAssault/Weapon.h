// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class BATTERYNASSAULT_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AWeapon();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	virtual void StartAttack();
	virtual void EndAttack();
	FORCEINLINE bool IsAttacking() { return bIsAttacking; }
	FORCEINLINE virtual bool CanAttack() { return TimeToNextAttack <= 0.f; }
	FORCEINLINE void turnoff() { bIsAttacking = false; }

	virtual void AddAmmo(float value) { AmmoUsed += value; }
protected:
	UPROPERTY(EditDefaultsOnly)
		float AttackSpeed;

	float TimeToNextAttack;

	int32 AmmoUsed = 0;

protected:
	virtual void Attack();
	//virtual bool CheckAmmo();

private:
	bool bIsAttacking;
	
	
};
