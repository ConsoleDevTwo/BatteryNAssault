// Fill out your copyright notice in the Description page of Project Settings.

#include "BatteryNAssault.h"
#include "PowerUp.h"


// Sets default values
APowerUp::APowerUp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	Collider->SetSphereRadius(55.f);
	RootComponent = Collider;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->AttachTo(RootComponent);
}

// Called when the game starts or when spawned
void APowerUp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APowerUp::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

// Called if power up pressed
void APowerUp::ExecutePowerUp(AActor * instigator)
{
	GEngine->AddOnScreenDebugMessage(
		1,
		1.f,
		FColor::Yellow,
		TEXT("PowerUp"));
	Destroy();
}

