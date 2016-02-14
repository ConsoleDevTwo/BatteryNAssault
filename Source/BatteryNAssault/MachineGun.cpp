// Fill out your copyright notice in the Description page of Project Settings.

#include "BatteryNAssault.h"
#include "MyProjectile.h"
#include "BatteryNAssaultCharacter.h"
#include "MachineGun.h"




AMachineGun::AMachineGun()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->AttachTo(RootComponent);
	AttackSpeed = 0.3f;
	AmmoUsed = 1;
	


}
void AMachineGun::Attack()
{
	Super::Attack();

	FActorSpawnParameters SpawnParameters;

	

	ABatteryNAssaultCharacter* Character = Cast<ABatteryNAssaultCharacter>(Instigator);
	if (Character)
	{
		SpawnParameters.Instigator = Character;
		GetWorld()->SpawnActor<AMyProjectile>(
			ProjectileClass,
			GetActorLocation(), 
			Instigator->GetActorRotation(), 
			SpawnParameters);
	}
	




}