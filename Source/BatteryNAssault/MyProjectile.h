// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "MyProjectile.generated.h"

UCLASS()
class BATTERYNASSAULT_API AMyProjectile : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AMyProjectile();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

protected:
	UPROPERTY(VisibleAnywhere)
		UProjectileMovementComponent* MovementComponent;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere)
		USphereComponent* Collider;
protected:
	UFUNCTION()
		virtual void OnBeginOverlap(AActor* OtherActor);

	UPROPERTY(EditDefaultsOnly)
		float DamageDealt = 1;


	UPROPERTY(VisibleAnywhere)
		FVector InitPosition;

	UPROPERTY(VisibleAnywhere)
		FVector Direction;
	
	
};
