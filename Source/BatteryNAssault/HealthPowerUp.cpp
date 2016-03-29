// Fill out your copyright notice in the Description page of Project Settings.

#include "BatteryNAssault.h"
#include "BatteryNAssaultCharacter.h"
#include "HealthPowerUp.h"



// Called if power up pressed
void AHealthPowerUp::ExecutePowerUp(AActor * instigator)
{
	//GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Yellow, TEXT("AHealthPowerUp PowerUp"));
	//Cast the object to the cahracter class
	ABatteryNAssaultCharacter* Character = Cast<ABatteryNAssaultCharacter>(instigator);
	// If this character exists check the team ID 
	if (Character)
	{
		Character->AddHealth(Character->MaxHealth);
	}
	Destroy();
}
