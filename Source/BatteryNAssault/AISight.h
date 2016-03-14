// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "AISight.generated.h"

UCLASS()
class BATTERYNASSAULT_API AAISight : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAISight();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	void SetAI(class AMechAICharacter* character);

protected:
	class AMechAICharacter* MyCharacter;

	USceneComponent* Root;

	UPROPERTY(EditAnywhere, Category = "AI")
	class UPawnSensingComponent* PawnSensingComp;
	
};
