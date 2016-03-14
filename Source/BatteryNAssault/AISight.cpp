// Fill out your copyright notice in the Description page of Project Settings.

#include "BatteryNAssault.h"
#include "AISight.h"

#include "MechAICharacter.h"
// AI include
#include "Perception/PawnSensingComponent.h"


// Sets default values
AAISight::AAISight()
{
	Root = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = Root;
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	PawnSensingComp->SetPeripheralVisionAngle(60.0f);
	PawnSensingComp->SightRadius = 2000;
	PawnSensingComp->HearingThreshold = 600;
	PawnSensingComp->LOSHearingThreshold = 1200;

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AAISight::BeginPlay()
{
	Super::BeginPlay();	
}

// Called every frame
void AAISight::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AAISight::SetAI(AMechAICharacter* character)
{
	MyCharacter = character;

	if (PawnSensingComp)
	{
		PawnSensingComp->OnSeePawn.AddDynamic(MyCharacter, &AMechAICharacter::OnSeePlayer);
	}
}
