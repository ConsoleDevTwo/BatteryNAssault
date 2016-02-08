// Fill out your copyright notice in the Description page of Project Settings.

#include "BatteryNAssault.h"
#include "MechAICharacter.h"
#include "MechAIController.h"
#include "BatteryNAssaultCharacter.h"


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
}

// Called when the game starts or when spawned
void AMechAICharacter::BeginPlay()
{
	Super::BeginPlay();

	if (PawnSensingComp)
	{
		PawnSensingComp->OnSeePawn.AddDynamic(this, &AMechAICharacter::OnSeePlayer);
	}

}

// Called every frame
void AMechAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMechAICharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}


void AMechAICharacter::OnSeePlayer(APawn* Pawn)
{
	AMechAIController* MechAIController = Cast<AMechAIController>(GetController());
	ABatteryNAssaultCharacter* Target = Cast<ABatteryNAssaultCharacter>(Pawn);
	if (MechAIController && Target)
	{
		MechAIController->SetTargetEnemy(Target);
	}
}



