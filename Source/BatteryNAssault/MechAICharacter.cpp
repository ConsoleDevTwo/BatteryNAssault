// Fill out your copyright notice in the Description page of Project Settings.

#include "BatteryNAssault.h"
#include "MechAICharacter.h"
#include "MechAIController.h"
#include "BatteryNAssaultCharacter.h"
#include "BaseBattery.h"

// AI include
#include "Perception/PawnSensingComponent.h"


// Sets default values
AMechAICharacter::AMechAICharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	PawnSensingComp->SetPeripheralVisionAngle(60.0f);
	PawnSensingComp->SightRadius = 2000;
	PawnSensingComp->HearingThreshold = 600;
	PawnSensingComp->LOSHearingThreshold = 1200;

	m_CurrentWaypoint = NULL;
	WaypointToPlayerDistance = 100.0f;
}

// Called when the game starts or when spawned
void AMechAICharacter::BeginPlay()
{
	Super::BeginPlay();
	FollowCamera->Deactivate();

	// Get all the wander waypoints in the map
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWanderWaypoint::StaticClass(), m_Waypoints);

	SelectWaypoint();

	if (PawnSensingComp)
	{
		PawnSensingComp->OnSeePawn.AddDynamic(this, &AMechAICharacter::OnSeePlayer);
		
	}

}

// Called every frame
void AMechAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_CurrentWaypoint == NULL || FVector::Dist(GetTransform().GetTranslation(), m_CurrentWaypoint->GetTransform().GetTranslation()) <= WaypointToPlayerDistance)
	{
		SelectWaypoint();
	}
	
}

// Called to bind functionality to input
void AMechAICharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

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
