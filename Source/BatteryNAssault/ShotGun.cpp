// Fill out your copyright notice in the Description page of Project Settings.

#include "BatteryNAssault.h"
#include "ShotGun.h"




AShotGun::AShotGun()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->AttachTo(RootComponent);
	AttackSpeed = 0.8f;
	AmmoUsed = 5;
	Range = 500.0f;
	DamageDealt = 1.0f;
}



void AShotGun::Attack()
{
	Super::Super::Attack();
	for (int32 i = 0; i <= Spread; i++)
	{


		const int32 RandomNumber = FMath::Rand();
		FRandomStream WeaponRandomStream(RandomNumber);
		const float CurrentSpread = Spread;
		const float SpreadCone = FMath::DegreesToRadians(Spread+2*0.5);
		FVector Start = GetActorLocation();

		FVector Direction = GetActorRotation().Vector();
		FVector BulletDirection = WeaponRandomStream.VRandCone(Direction, SpreadCone, SpreadCone);
		FVector End = Start + BulletDirection * Range;

		FHitResult Impact = WeaponTrace(Start, End);

		HitResult(Impact, Start, BulletDirection);
	}
}
