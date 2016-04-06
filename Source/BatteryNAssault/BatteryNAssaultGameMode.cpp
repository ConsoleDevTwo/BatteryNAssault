// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "BatteryNAssault.h"
#include "BatteryNAssaultGameMode.h"
#include "BatteryNAssaultCharacter.h"

ABatteryNAssaultGameMode::ABatteryNAssaultGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}


void ABatteryNAssaultGameMode::Tick(float DeltaSeconds)
{
	TArray<AActor *> gameMechs;
	
	int TeamOne = 0;
	int TeamZero = 0;
	
	int Winner = -1;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABatteryNAssaultCharacter::StaticClass(), gameMechs);
	for (int i = 0; i < gameMechs.Num(); i++)
	{
		//Cast the object to the cahracter class
		ABatteryNAssaultCharacter* Character = Cast<ABatteryNAssaultCharacter>(gameMechs[i]);
		// If this character exists check the team ID 
		if (Character)
		{
			// If it's the same team ID, recharge the battery
			if (Character->TeamID == 1 && !Character->DeathState)
			{
				TeamOne++;
			}
			else if (Character->TeamID == 0 && !Character->DeathState)
			{
				TeamZero++;
			}
		}
	}

	if (TeamOne == 0)
	{
		Winner = 0;
	}
	else if(TeamZero == 0)
	{
		Winner = 1;
	}

	if (Winner == -1)
	{
		return;
	}

	APlayerController* controller = GetWorld()->GetFirstPlayerController();
	if (controller)
	{
		ABatteryNAssaultCharacter *mech = Cast<ABatteryNAssaultCharacter>(controller->GetPawn());
		if(mech)
		{
			if (mech->TeamID == Winner)
			{
				FString Message = "GameOver - Winner";
				GEngine->AddOnScreenDebugMessage(1, 0.1f, FColor::White, Message);
			}
			else
			{
				FString Message = "GameOver - Loser";
				GEngine->AddOnScreenDebugMessage(1, 0.1f, FColor::White, Message);
			}
		}
	}
}
