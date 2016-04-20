// Fill out your copyright notice in the Description page of Project Settings.

#include "BatteryNAssault.h"
#include "MyProjectile.h"
#include "BatteryNAssaultCharacter.h"
#include "MachineGun.h"




AMachineGun::AMachineGun()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->AttachTo(RootComponent);
	AttackSpeed = 0.05f;
	AmmoUsed = 1;
	
	ConstructorHelpers::FObjectFinder<USoundCue> MachineGunSound(TEXT("SoundCue'/Game/MAARS/Sounds/S_Shot_Cue.S_Shot_Cue'"));
	FiringSound = MachineGunSound.Object;
	AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComp"));
	AudioComp->AttachTo(RootComponent);
	AudioComp->bAutoActivate = false;

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
		if (FiringSound)
		{

			AudioComp = UGameplayStatics::SpawnSoundAttached(FiringSound, this->GetRootComponent());
		}
	}
	




}