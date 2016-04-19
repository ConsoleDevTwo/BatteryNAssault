// Fill out your copyright notice in the Description page of Project Settings.

#include "BatteryNAssault.h"
#include "MyProjectile.h"
#include "BatteryNAssaultCharacter.h"
#include "RocketLauncher.h"
#include <time.h>


/* Rocket Launcher is actually Shotgun */

ARocketLauncher::ARocketLauncher()
{
	srand(time(NULL));

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->AttachTo(RootComponent);
	AttackSpeed = 0.5f;
	
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

		FRotator rotation = this->GetActorRotation();
		FVector location = GetActorLocation();
		float min = -5;
		float max = 5;

		for (int i = 0; i < 5; i++)
		{
			float r = min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
			float r2 = min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));

			rotation = this->GetActorRotation();
			rotation.Add(0.0f, r * i, r2 * i);

			GetWorld()->SpawnActor<AMyProjectile>(
				ProjectileClass,
				location,
				rotation,
				SpawnParameters);
		}

		if (FiringSound)
		{
			AudioComp = UGameplayStatics::SpawnSoundAttached(FiringSound, this->GetRootComponent());
		}
	}





}