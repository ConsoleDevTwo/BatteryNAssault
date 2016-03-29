// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "Weapon.h"
#include "BatteryNAssaultCharacter.generated.h"

UCLASS(config=Game)
class ABatteryNAssaultCharacter : public ACharacter
{
	GENERATED_BODY()
public:
	ABatteryNAssaultCharacter();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;


	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	UPROPERTY(EditAnywhere)
		AWeapon* Weapon;

	UFUNCTION()
	virtual void Recharge ( float charge );

	virtual float TakeDamage(
				float DamageAmount,
				struct FDamageEvent const& DamageEvent,
				class AController* EventInstigator,
				class AActor* DamageCauser
			) override;

protected:

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	UPROPERTY(EditAnywhere)
	float EnergyCostPerSecond;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float Energy;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float Health;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AWeapon> Gun;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USkeletalMeshComponent* Turret;

	UFUNCTION()
	virtual void StartFire();

	UFUNCTION()
	virtual void StopFire();

	UFUNCTION()
	virtual void PowerUp();

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface

	// Array of Power up objects
	TArray<AActor *> powerUpMechs;

private:
	//Changes the color of the robot depending on the team ID
	UFUNCTION()
	void ChangeRobotColor();

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float MaxHealth;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float MaxEnergy;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	// The team ID of this player
	UPROPERTY(EditAnywhere, Category = "Team")
	int8 TeamID;

	UPROPERTY(EditAnywhere, Category = "Tower")
	FRotator TowerRotation;

	UFUNCTION()
	float GetEnergy();

	void AddHealth(float value) { 
		Health = FMath::Clamp(Health + value, 0.0f, MaxHealth);
	}
	void AddAmmo(float value) { Weapon->AddAmmo(value); }
};

