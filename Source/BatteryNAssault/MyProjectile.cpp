// Fill out your copyright notice in the Description page of Project Settings.

#include "BatteryNAssault.h"
#include "MyTestTarget.h"
#include "MyProjectile.h"


// Sets default values
AMyProjectile::AMyProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	Collider->SetSphereRadius(5.f);
	RootComponent = Collider;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->AttachTo(Collider);

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComponent"));
	OnActorBeginOverlap.AddDynamic(this, &AMyProjectile::OnBeginOverlap);

	InitialLifeSpan = 10;
	

}

// Called when the game starts or when spawned
void AMyProjectile::BeginPlay()
{
	Super::BeginPlay();
	Direction = GetActorForwardVector();
	InitPosition = GetActorLocation();
}

// Called every frame
void AMyProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector Distatnce = GetActorLocation() - InitPosition;
	if (Distatnce.Size() > 2000)
	{
		Destroy();
	}
}
void AMyProjectile::OnBeginOverlap(AActor* OtherActor)
{
	AMyTestTarget *Target = Cast<AMyTestTarget>(OtherActor);

	if (Target)
	{
		GEngine->AddOnScreenDebugMessage(
			1,
			1.f,
			FColor::Yellow,
			TEXT("destroy"));

		UGameplayStatics::ApplyDamage(
			OtherActor, 
			DamageDealt, 
			Instigator->GetController(), 
			Instigator, 
			UDamageType::StaticClass());
		OtherActor->Destroy();
		Destroy();
	}
	//Destroy();
}

