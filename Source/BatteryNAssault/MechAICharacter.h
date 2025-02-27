// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BatteryNAssaultCharacter.h"
#include "WanderWaypoint.h"
#include "MechAICharacter.generated.h"


UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class AIStates : uint8
{
	SEARCHING 	UMETA(DisplayName = "Searching"),
	SPOTTED  UMETA(DisplayName = "EnemySpotted"),
};

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

	FRotator FindLookRotation();

protected:
	UFUNCTION()
	void SelectWaypoint();

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float WaypointToPlayerDistance;

private:
	UPROPERTY(VisibleAnywhere, Category = "AI")
	AActor* m_CurrentWaypoint;

	UPROPERTY(VisibleAnywhere, Category = "AI")
	TArray<AActor*> m_Waypoints;

	// Finds a new random location around the mech to look towards
	UFUNCTION()
	void FindNewLookLocation();

	// Rotates the tower to look towards the location
	UFUNCTION()
	void RotateTower(float DeltaTime);

	UFUNCTION()
	void AttemptToFire();

public:
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	class UBehaviorTree* BehaviorTree;

	UPROPERTY(VisibleAnywhere)
	class AAISight* AISightComp;

	// The speed at which the tower will rotate
	UPROPERTY(EditAnywhere, Category = "Tower")
	float TowerRotationSpeed;

	// The direction that the AI will try to look at
	UPROPERTY(EditAnywhere, Category = "Tower")
	FVector LookLocation;

	// If the AI has looked at where it wanted to it will be set to true
	UPROPERTY(EditAnywhere, Category = "Tower")
	bool bIsAtLookDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	AIStates State;

	UPROPERTY(VisibleAnywhere, Category = "Energy")
	bool bIsCharging;
	
	UPROPERTY(VisibleAnywhere, Category = "Control")
	bool IsStunned;
	
	UPROPERTY(VisibleAnywhere, Category = "Control")
	float StunTime;

	UPROPERTY(VisibleAnywhere, Category = "Control")
	bool Controlled;

	// Triggered by the UPawnSensing component when any pawn is spotted
	UFUNCTION()
	void OnSeePlayer(APawn* Pawn);
};
