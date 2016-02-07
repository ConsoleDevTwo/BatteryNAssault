// Fill out your copyright notice in the Description page of Project Settings.

#include "BatteryNAssault.h"
#include "Weapon.h"


// Sets default values
AWeapon::AWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TimeToNextAttack -= DeltaTime;
	if (CanAttack() && IsAttacking())
	{
		Attack();
	}
}

void AWeapon::StartAttack()
{
	bIsAttacking = true;
}

void AWeapon::EndAttack()
{
	bIsAttacking = false;
}

void AWeapon::Attack()
{
	TimeToNextAttack = AttackSpeed;


}
/*
bool AWeapon::CheckAmmo()
{
	AMyPlayerState *PlayerState = Cast<AMyPlayerState>(Instigator->PlayerState);
	if (PlayerState->Ammo < AmmoUsed)
		return false;
	PlayerState->Ammo -= AmmoUsed;
	return true;
}
*/
