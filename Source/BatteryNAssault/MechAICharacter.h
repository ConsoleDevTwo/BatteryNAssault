// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BatteryNAssaultCharacter.h"
#include "WanderWaypoint.h"
#include "MechAICharacter.generated.h"

/**
 * 
 */
UCLASS()
class BATTERYNASSAULT_API AMechAICharacter : public ABatteryNAssaultCharacter
{
	GENERATED_BODY()
	
public:
	// Sets default values for this character's properties
	AMechAICharacter();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	UPROPERTY(VisibleAnywhere, Category = "AI")
	class UPawnSensingComponent* PawnSensingComp;

protected:
	// Triggered by the UPawnSensing component when any pawn is spotted
	UFUNCTION()
	void OnSeePlayer(APawn* Pawn);

	UFUNCTION()
	void SelectWaypoint();

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float WaypointToPlayerDistance;


private:
	UPROPERTY(VisibleAnywhere, Category = "AI")
	AActor* m_CurrentWaypoint;

	UPROPERTY(VisibleAnywhere, Category = "AI")
	TArray<AActor*> m_Waypoints;

public:
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	class UBehaviorTree* BehaviorTree;


};
