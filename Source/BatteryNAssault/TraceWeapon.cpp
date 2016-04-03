// Fill out your copyright notice in the Description page of Project Settings.

#include "BatteryNAssault.h"
#include "TraceWeapon.h"




ATraceWeapon::ATraceWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
	


}
void ATraceWeapon::Attack()
{
	Super::Attack();
	

	FVector Start = GetActorLocation();

	FVector Direction = GetActorRotation().Vector();
	FVector End = Start + Direction * Range;

	FHitResult Impact = WeaponTrace(Start, End);

	HitResult(Impact, Start, Direction);

	


}
FHitResult ATraceWeapon::WeaponTrace(FVector &TraceFrom, FVector &TraceTo)
{
	FCollisionQueryParams Trace("Gun", true, Instigator);
	Trace.bTraceAsyncScene = true;
	Trace.bReturnPhysicalMaterial = true;
	Trace.AddIgnoredActor(this);
	Trace.AddIgnoredActor(Instigator);
	FHitResult Hit(ForceInit);
	GetWorld()->LineTraceSingleByChannel(Hit, TraceFrom, TraceTo, ECC_Visibility, Trace);
	return Hit;

}
void ATraceWeapon::HitResult(FHitResult Impact, const FVector Origin, FVector ShootDir)
{
	FVector EndTrace = Origin + ShootDir * Range;
	FVector EndPoint = Impact.GetActor() ? Impact.ImpactPoint : EndTrace;
	DrawDebugLine(this->GetWorld(), Origin, Impact.TraceEnd, FColor::Blue, false, 0.2f, 10.f, 2.f);



	if (Impact.GetActor())
	{


		UGameplayStatics::ApplyDamage(Impact.GetActor(), DamageDealt, Instigator->GetController(), Instigator, UDamageType::StaticClass());

	}
}
