// Fill out your copyright notice in the Description page of Project Settings.

#include "BatteryNAssault.h"
#include "GrenadeLauncher.h"
#include "Grenade.h"



void AGrenade::OnConstruction(const FTransform & Transform)
{
	Super::OnConstruction(Transform);
	AGrenadeLauncher* GrenadeLauncher = Cast<AGrenadeLauncher>(GetOwner());
	if (GrenadeLauncher)
	{
		MovementComponent->InitialSpeed = GrenadeLauncher->GrenadeSpeed;

	}

}



void AGrenade::OnBeginOverlap(AActor* OtherActor)
{



	if (flame)
		UGameplayStatics::SpawnEmitterAtLocation(this, flame, GetActorLocation());
	UGameplayStatics::ApplyRadialDamage(this, DamageDealt, GetActorLocation(), 250, UDamageType::StaticClass(), TArray<AActor*>(), Instigator, Instigator->GetController(), true);
	Destroy();
}




