// Fill out your copyright notice in the Description page of Project Settings.

#include "BatteryNAssault.h"
#include "MyProjectile.h"
#include "BatteryNAssaultCharacter.h"
#include "RocketLauncher.h"




ARocketLauncher::ARocketLauncher()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->AttachTo(RootComponent);
	AttackSpeed = 0.1f;
	



}
void ARocketLauncher::Attack()
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
			this->GetActorRotation(),
			SpawnParameters);
	}





}