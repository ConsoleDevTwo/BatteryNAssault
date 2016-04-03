// Fill out your copyright notice in the Description page of Project Settings.

#include "BatteryNAssault.h"
#include "MyProjectile.h"
#include "BatteryNAssaultCharacter.h"
#include "RocketLauncher.h"




ARocketLauncher::ARocketLauncher()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->AttachTo(RootComponent);
	AttackSpeed = 1.0f;
	
	ConstructorHelpers::FObjectFinder<USoundCue> RocketLauncherSound(TEXT("SoundCue'/Game/Sound/Weapon/machine1cue.machine1cue'"));
	FiringSound = RocketLauncherSound.Object;
	AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComp"));
	AudioComp->AttachTo(RootComponent);
	AudioComp->bAutoActivate = false;


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
		if (FiringSound)
		{

			AudioComp = UGameplayStatics::SpawnSoundAttached(FiringSound, this->GetRootComponent());
		}
	}





}