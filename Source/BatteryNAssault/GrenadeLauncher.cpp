// Fill out your copyright notice in the Description page of Project Settings.

#include "BatteryNAssault.h"
#include "BatteryNAssaultCharacter.h"
#include "Grenade.h"
#include "GrenadeLauncher.h"

#define MAX_GRENADE_SPEED 1500

AGrenadeLauncher::AGrenadeLauncher()
{
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->AttachTo(RootComponent);
	AttackSpeed = 0.5f;//How fast the power increases
	TimeBetweenShots = 1.f;//the how fast the player can shoot
	GrenadeSpeed = 500;
	AmmoUsed = 10;
}

void AGrenadeLauncher::Attack()
{
	Super::Attack();
	GrenadeSpeed = FMath::Clamp<float>(GrenadeSpeed + 50, 0, MAX_GRENADE_SPEED);
}

void AGrenadeLauncher::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//TimeLeft -= DeltaTime;
}

void AGrenadeLauncher::EndAttack()
{
	FActorSpawnParameters SpawnParameters;

	ABatteryNAssaultCharacter* Character = Cast<ABatteryNAssaultCharacter>(Instigator);
	
	if (Character) //TimeLeft <= 0 
	{
		SpawnParameters.Owner = this;
		SpawnParameters.Instigator = Character;
		//GetWorld()->SpawnActor<AGrenade>(ProjectileClass, Mesh->GetSocketLocation("Muzzle"), Instigator->GetActorRotation(), SpawnParameters);
		//TimeLeft = TimeBetweenShots;

		SpawnParameters.Instigator = Character;
		GetWorld()->SpawnActor<AGrenade>(
			ProjectileClass,
			GetActorLocation(),
			Instigator->GetActorRotation(),
			SpawnParameters);
	}

	Super::EndAttack();
	//GrenadeSpeed = 30;
}





