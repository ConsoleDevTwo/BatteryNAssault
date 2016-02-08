// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include <vector>
#include "BatteryNAssaultCharacter.h"
#include "EngineUtils.h"
#include "BaseBattery.generated.h"

UCLASS()
class BATTERYNASSAULT_API ABaseBattery : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseBattery();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

protected:

	UPROPERTY(EditAnywhere)
	float ChargePerSecond;

	TArray<AActor *> gameMechs;
};
