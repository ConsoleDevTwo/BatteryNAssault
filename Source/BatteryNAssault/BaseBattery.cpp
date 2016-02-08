// Fill out your copyright notice in the Description page of Project Settings.

#include "BatteryNAssault.h"
#include "BaseBattery.h"


// Sets default values
ABaseBattery::ABaseBattery()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ChargePerSecond = 5.0f;
	ChargeRange = 400.0f;
}

// Called when the game starts or wheasn spawned
void ABaseBattery::BeginPlay()
{
	Super::BeginPlay();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABatteryNAssaultCharacter::StaticClass(), gameMechs);
}

// Called every frame
void ABaseBattery::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	for (int i = 0; i < gameMechs.Num(); i++)
	{
		if (FVector::Dist(gameMechs[i]->GetTransform().GetTranslation(), GetTransform().GetTranslation()) <= 400.0f)
		{
			FString Message = gameMechs[i]->GetName();
			GEngine->AddOnScreenDebugMessage(i + 1, 0.1f, FColor::White, Message);

			Cast<ABatteryNAssaultCharacter>(gameMechs[i])->Recharge(ChargePerSecond*DeltaTime);	
		}
		// Recharge gameMechs[i]->
	}
}

