// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "AISight.generated.h"

UCLASS()
class BATTERYNASSAULT_API AAISight : public APawn
{
	GENERATED_BODY()
public:	
	// Sets default values for this actor's properties
	AAISight();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick(float DeltaSeconds) override;
	
	UPROPERTY(EditAnywhere, Category = "AI")
	class UPawnSensingComponent* PawnSensingComp;

protected:
	USceneComponent* Root;


};
