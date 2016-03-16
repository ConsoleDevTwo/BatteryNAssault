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
}

// Called when the game starts or when spawned
void AMechAICharacter::BeginPlay()
{
	Super::BeginPlay();

	FActorSpawnParameters spawnParams;
	spawnParams.Owner = this;

	// Give the AI a AISight component and attach it to the head
	AISightComp = GetWorld()->SpawnActor<AAISight>(AAISight::StaticClass(), spawnParams);
	AISightComp->AttachRootComponentTo(this->GetMesh(), FName("S_WEAPON"));
	AISightComp->SetActorRelativeRotation(FRotator(0, 180, 0));
	// Tell the pawn sensing component to use our OnSeePlayer Function that we created
	AISightComp->PawnSensingComp->OnSeePawn.AddDynamic(this, &AMechAICharacter::OnSeePlayer);

	// Deactivate the camera since we are not a player
	FollowCamera->Deactivate();

	// Get all the wander waypoints in the map
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWanderWaypoint::StaticClass(), m_Waypoints);

	SelectWaypoint();
}

// Called every frame
void AMechAICharacter::Tick(float DeltaTime)
{
	if (Energy > 0)
	{
		Energy -= EnergyCostPerSecond * DeltaTime;
	}

	if (m_CurrentWaypoint == NULL || FVector::Dist(GetTransform().GetTranslation(), m_CurrentWaypoint->GetTransform().GetTranslation()) <= WaypointToPlayerDistance)
	{
		SelectWaypoint();
	}

	RotateTower(DeltaTime);
	FindNewLookLocation();

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

void AMechAICharacter::RotateTower(float DeltaTime)
{

	FVector Direction = LookLocation - GetActorLocation();
	FRotator rot = FRotationMatrix::MakeFromX(Direction).Rotator();

	if (TowerRotation.Yaw + 5.0f < rot.Yaw || TowerRotation.Yaw - 5.0f > rot.Yaw)
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