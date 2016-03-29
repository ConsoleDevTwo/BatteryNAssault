// Fill out your copyright notice in the Description page of Project Settings.

#include "BatteryNAssault.h"
#include "BatteryNAssaultCharacter.h"
#include "AmmoPowerUp.h"


// Called if power up pressed
void AAmmoPowerUp::ExecutePowerUp(AActor * instigator)
{
	//GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Yellow, TEXT("AAmmoPowerUp PowerUp"));
	//Cast the object to the cahracter class
	ABatteryNAssaultCharacter* Character = Cast<ABatteryNAssaultCharacter>(instigator);
	// If this character exists check the team ID 
	if (Character)
	{
		Character->AddAmmo(10);
	}
	Destroy();
}

