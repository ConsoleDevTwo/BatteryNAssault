// Fill out your copyright notice in the Description page of Project Settings.

#include "BatteryNAssault.h"
#include "Rocket.h"

#define SPEEDINCREASE 50
#define TIME_T0_SPEEDINCREASE 0.05


void ARocket::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Acceleration -= DeltaTime;
	if (Acceleration <= 0.f)
	{
		Speed += SPEEDINCREASE;
		MovementComponent->SetVelocityInLocalSpace(FVector(Speed,0,0));
		Acceleration = TIME_T0_SPEEDINCREASE;
	}
	
}

void ARocket::OnBeginOverlap(AActor* OtherActor)
{



	if (flame)
		UGameplayStatics::SpawnEmitterAtLocation(this, flame, GetActorLocation());
	UGameplayStatics::ApplyRadialDamage(this, DamageDealt, GetActorLocation(), 250, UDamageType::StaticClass(), TArray<AActor*>(), Instigator, Instigator->GetController(), true);
	Destroy();
}




