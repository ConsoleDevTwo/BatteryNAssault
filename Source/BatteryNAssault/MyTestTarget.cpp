// Fill out your copyright notice in the Description page of Project Settings.

#include "BatteryNAssault.h"
#include "MyProjectile.h"
#include "MyTestTarget.h"


// Sets default values
AMyTestTarget::AMyTestTarget()
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
/*void AMyTestTarget::BeginPlay()
{
	Super::BeginPlay();
	
}
*/
// Called every frame
/*void AMyTestTarget::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}
*/
