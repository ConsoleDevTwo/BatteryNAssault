// Fill out your copyright notice in the Description page of Project Settings.

#include "BatteryNAssault.h"
#include "BatteryNAssaultCharacter.h"
#include "EnergyPowerUp.h"


// Called if power up pressed
void AEnergyPowerUp::ExecutePowerUp(AActor * instigator)
{
	//GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Yellow, TEXT("AEnergy PowerUp"));

	//Cast the object to the cahracter class
	ABatteryNAssaultCharacter* Character = Cast<ABatteryNAssaultCharacter>(instigator);
	// If this character exists check the team ID 
	if (Character)
	{
		Character->Recharge(Character->MaxEnergy);
	}
	Destroy();
}

