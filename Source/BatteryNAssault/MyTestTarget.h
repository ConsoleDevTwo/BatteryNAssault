// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "MyTestTarget.generated.h"

UCLASS()
class BATTERYNASSAULT_API AMyTestTarget : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyTestTarget();

	// Called when the game starts or when spawned
	//virtual void BeginPlay() override;
	
	// Called every frame
	//virtual void Tick( float DeltaSeconds ) override;

	virtual float TakeDamage (
			float DamageAmount,
			struct FDamageEvent const& DamageEvent,
			class AController* EventInstigator,
			class AActor* DamageCauser
		) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		USphereComponent* Collider;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UStaticMeshComponent* Mesh;
	
};
