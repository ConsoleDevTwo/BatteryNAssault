// Fill out your copyright notice in the Description page of Project Settings.

#include "BatteryNAssault.h"
#include "MechAICharacter.h"
#include "MechAIController.h"
#include "BatteryNAssaultCharacter.h"
#include "BaseBattery.h"
#include "AISight.h"
#include "Perception/PawnSensingComponent.h"

// Sets default values
AMechAICharacter::AMechAICharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_CurrentWaypoint = NULL;
	WaypointToPlayerDistance = 100.0f;

	TowerRotationSpeed = 5.0f;
	IsStunned = false;
	StunTime = 0;
}

// Called when the game starts or when spawned
void AMechAICharacter::BeginPlay()
{
	Super::BeginPlay();

	FActorSpawnParameters spawnParams;
	spawnParams.Owner = this;
	
	//attach the turret to the root component
	Turret->AttachTo(this->GetMesh(), FName("S_WEAPON"));

	// Give the AI a AISight component and attach it to the head
	AISightComp = GetWorld()->SpawnActor<AAISight>(AAISight::StaticClass(), spawnParams);
	AISightComp->AttachRootComponentTo(this->GetMesh(), FName("S_WEAPON"));
	AISightComp->SetActorRelativeRotation(FRotator(0, 180, 0));
	// Tell the pawn sensing component to use our OnSeePlayer Function that we created
	AISightComp->PawnSensingComp->OnSeePawn.AddDynamic(this, &AMechAICharacter::OnSeePlayer);

	// Deactivate the camera since we are not a player
	FollowCamera->Deactivate();
	
	bIsCharging = false;
	Controlled = false;

	// Get all the wander waypoints in the map
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWanderWaypoint::StaticClass(), m_Waypoints);

	SelectWaypoint();
}

// Called every frame
void AMechAICharacter::Tick(float DeltaTime)
{
	if (Controlled)
	{
		Super::Tick(DeltaTime);
		return;
	}

	if (Energy > 0)
	{
		Energy -= EnergyCostPerSecond * DeltaTime;
	}
	else
	{
		CharacterMovement->MaxWalkSpeed = 0;
	}

	if (IsStunned)
	{
		StunTime += DeltaTime;
		if (StunTime >= 1)
		{
			IsStunned = false;
			StunTime = 0;
		}
	}

	if (Health <= 0)
	{
		Destroy();
	}

	switch (State)
	{
	case AIStates::SEARCHING:
		FindNewLookLocation();

		if (m_CurrentWaypoint == NULL || FVector::Dist(GetTransform().GetTranslation(), m_CurrentWaypoint->GetTransform().GetTranslation()) <= WaypointToPlayerDistance)
		{
			SelectWaypoint();
		}
		break;
	case AIStates::SPOTTED:
		AttemptToFire();
		break;
	default:
		break;
	}

	RotateTower(DeltaTime);

}

// What the AI will do when it sees a player
void AMechAICharacter::OnSeePlayer(APawn* Pawn)
{
	AMechAIController* MechAIController = Cast<AMechAIController>(GetController());
	ABatteryNAssaultCharacter* Target = Cast<ABatteryNAssaultCharacter>(Pawn);
	if (MechAIController && Target)
	{
		MechAIController->SetTargetEnemy(Target);
	}
}

void AMechAICharacter::SelectWaypoint()
{
	if (m_Waypoints.Num() <= 0)
	{
		return;
	}

	AMechAIController* MechAIController = Cast<AMechAIController>(GetController());
	if (MechAIController)
	{
		AActor* waypoint = m_Waypoints[FMath::RandRange(0, m_Waypoints.Num() - 1)];
		m_CurrentWaypoint = waypoint;
		MechAIController->SetNextWaypoint(waypoint);
	}
}

FRotator AMechAICharacter::FindLookRotation()
{
	FVector Direction = LookLocation - GetActorLocation();
	return FRotationMatrix::MakeFromX(Direction).Rotator() - GetActorRotation();
}

void AMechAICharacter::RotateTower(float DeltaTime)
{
	FRotator rot = FindLookRotation();

	const FRotator ActorRot = GetActorRotation();
	const FRotator TurretRotation = rot;;

	if (Turret->GetComponentRotation() != TurretRotation)
	{
		FRotator currentCameraRotation = FMath::RInterpTo(Turret->GetComponentRotation(), TurretRotation, GetWorld()->GetDeltaSeconds(), 3.0f);
	//	Turret->SetWorldRotation(currentCameraRotation);
	}

	if (TowerRotation.Yaw +10.0f < rot.Yaw || TowerRotation.Yaw - 10.0f > rot.Yaw)
	{
		// If the bool is true, set it to false
		if (bIsAtLookDirection)
			bIsAtLookDirection = false;

		// Rotate the tower
		TowerRotation = FMath::RInterpTo(TowerRotation, rot, DeltaTime, TowerRotationSpeed);
	}
	else if (!bIsAtLookDirection)
	{
		bIsAtLookDirection = true;
	}
}

void AMechAICharacter::FindNewLookLocation()
{
	if (!bIsAtLookDirection)
	{
		return;
	}

	float rand = FMath::Rand() % 100;

	LookLocation = GetActorLocation() + FVector(FMath::Sin(rand) * 2000, FMath::Cos(rand) * 2000, 0);
	bIsAtLookDirection = false;
}

void AMechAICharacter::AttemptToFire()
{
	if (!bIsAtLookDirection)
	{
		StopFire();
		return;
	}
	
	StartFire();
}