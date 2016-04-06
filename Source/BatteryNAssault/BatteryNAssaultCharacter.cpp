// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "BatteryNAssault.h"
#include "BatteryNAssaultCharacter.h"
#include "MechAICharacter.h"
#include "PowerUp.h"

//////////////////////////////////////////////////////////////////////////
// ABatteryNAssaultCharacter

ABatteryNAssaultCharacter::ABatteryNAssaultCharacter()
{
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinder<UClass> MachineGun;
		//FConstructorStatics() : MachineGun(TEXT("Class'/Game/Weapon/ProjectileWeapons/MachineGun.MachineGun_C'")) {}
		FConstructorStatics() : MachineGun(TEXT("Class'/Game/Weapon/ProjectileWeapons/RocketLauncher/RocketLauncher.RocketLauncher_C'")) {}
	};
	static FConstructorStatics ConstructorStatics;

	if (ConstructorStatics.MachineGun.Object)
	{
		Gun = Cast<UClass>(ConstructorStatics.MachineGun.Object);
	}
	//Temp->K2_SetWorldRotation(FollowCamera.)

	EnergyCostPerSecond = 0.5f;
	MaxEnergy = 100.f;
	Energy = MaxEnergy;

	MaxHealth = 100.f;
	Health = MaxHealth;
	DeathState = false;

	bInEnemySight = false;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->AttachTo(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	Turret = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Turret"));
	Turret->AttachTo(RootComponent);

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->AttachTo(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	TeamID = 0;
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

void ABatteryNAssaultCharacter::BeginPlay()
{
	Super::BeginPlay();

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = this;
	SpawnParameters.Instigator = this;
	AWeapon *Spawn = GetWorld()->SpawnActor<AWeapon>(Gun, SpawnParameters);
	if (Spawn)
	{
		Spawn->AttachRootComponentTo(Turret,"FirePoint", EAttachLocation::SnapToTarget);
		//Spawn->AttachRootComponentTo(Turret);
		Weapon = Spawn;
	}
	

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APowerUp::StaticClass(), powerUpMechs);
	ChangeRobotColor();
}

void ABatteryNAssaultCharacter::DeathFunc()
{
	if (DeathState) return;

	DeathState = true;
	Health = 0;
	Energy = 0;
}

void ABatteryNAssaultCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (DeathState) 
		return;
	if (Energy > 0)
	{
		Energy -= EnergyCostPerSecond * DeltaTime;
	}

	if (Health <= 0)
	{
		PossessNewMech();
	}

	FString Message = FString::Printf(TEXT("Energy: %.2f"), Energy);
	GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::White, Message);
	const FRotator ActorRot = GetActorRotation();
	const FRotator BaseRotation(0, CameraBoom->GetComponentRotation().Yaw - ActorRot.Yaw, 0);
	const FRotator TurretRotation(-CameraBoom->GetComponentRotation().Pitch - ActorRot.Pitch, CameraBoom->GetComponentRotation().Yaw-180, 0);
	if (Turret->GetComponentRotation() != TurretRotation)
	{
		FRotator currentCameraRotation = FMath::RInterpTo(Turret->GetComponentRotation(), TurretRotation, GetWorld()->GetDeltaSeconds(), 3.0f);
		Turret->SetWorldRotation(currentCameraRotation);
	}
	if (TowerRotation != BaseRotation)
	{
		FRotator currentBaseRotation = FMath::RInterpTo(TowerRotation, BaseRotation, GetWorld()->GetDeltaSeconds(), 3.0f);
		TowerRotation = currentBaseRotation;
	}

	//const FRotator CameraRot = FollowCamera->GetComponentRotation();

}

//////////////////////////////////////////////////////////////////////////
// Input

void ABatteryNAssaultCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// Set up gameplay key bindings
	check(InputComponent);

	InputComponent->BindAxis("MoveForward", this, &ABatteryNAssaultCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ABatteryNAssaultCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("TurnRate", this, &ABatteryNAssaultCharacter::TurnAtRate);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	InputComponent->BindAxis("LookUpRate", this, &ABatteryNAssaultCharacter::LookUpAtRate);

	InputComponent->BindAction(TEXT("Fire"),
		IE_Pressed,
		this,
		&ABatteryNAssaultCharacter::StartFire);

	InputComponent->BindAction(TEXT("Fire"),
		IE_Released,
		this,
		&ABatteryNAssaultCharacter::StopFire);

	InputComponent->BindAction(TEXT("PowerUp"),
		IE_Pressed,
		this,
		&ABatteryNAssaultCharacter::PowerUp);
	
}

void ABatteryNAssaultCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ABatteryNAssaultCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ABatteryNAssaultCharacter::MoveForward(float Value)
{
	if (Energy > 0)
	{
		CharacterMovement->AddInputVector(GetActorForwardVector() *	CharacterMovement->MaxWalkSpeed * Value * GetWorld()->GetDeltaSeconds());
	}
	/*
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = GetActorRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
	*/
}

void ABatteryNAssaultCharacter::MoveRight(float Value)
{
	AddActorWorldRotation(FRotator(0.0f, Value * BaseTurnRate * GetWorld()->GetDeltaSeconds(), 0));
	/*
	if ( (Controller != NULL) && (Value != 0.0f) )
	{

		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
	*/

}

void ABatteryNAssaultCharacter::StartFire()
{
	Weapon->StartAttack();
}

void ABatteryNAssaultCharacter::StopFire()
{
	Weapon->EndAttack();
}

void ABatteryNAssaultCharacter::PowerUp()
{
	for (int i = 0; i < powerUpMechs.Num(); i++)
	{
		//Cast the object to the power up class
		APowerUp* PowerUpMech = Cast<APowerUp>(powerUpMechs[i]);
		// If power up 
		if (PowerUpMech)
		{
			// check the distanse
			if (FVector::Dist(PowerUpMech->GetTransform().GetTranslation(), GetTransform().GetTranslation()) <=250.0f)
			{
				const bool bAllowShrinking = true;
				powerUpMechs.RemoveAt(i, 1, bAllowShrinking);
				PowerUpMech->ExecutePowerUp(this);
			}
		}
	}
}

void ABatteryNAssaultCharacter::Recharge(float charge)
{
	//Energy += charge;
	Energy = FMath::Clamp(Energy + charge, 0.0f, MaxEnergy);

	FString Message = FString::Printf(TEXT("Charging"));
	GEngine->AddOnScreenDebugMessage(4, 0.1f, FColor::White, Message);
}

float ABatteryNAssaultCharacter::GetEnergy()
{
	return Energy;
}

float ABatteryNAssaultCharacter::TakeDamage(
	float DamageAmount,
	struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator,
	class AActor* DamageCauser)
{
	GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Yellow, TEXT("destroy"));
	//Destroy();


	ABatteryNAssaultCharacter* damageDealer = Cast<ABatteryNAssaultCharacter>(DamageCauser);

	if (damageDealer)
	{
		if (damageDealer->TeamID != this->TeamID)
		{	
			Health -= 5;
		}
	}

	return 0;
}

void ABatteryNAssaultCharacter::ChangeRobotColor()
{
	// Create a new dynamic material for this robot
	UMaterialInstanceDynamic* DynamicMat = UMaterialInstanceDynamic::Create(GetMesh()->GetMaterial(0), this);

	// Setting robot color based on team
	if (TeamID == 0)
	{
		DynamicMat->SetVectorParameterValue("Color", FLinearColor::Blue);
	}
	else if (TeamID == 1)
	{
		DynamicMat->SetVectorParameterValue("Color", FLinearColor::Red);
	}

	// Set the robots material to the new dynamic material
	GetMesh()->SetMobility(EComponentMobility::Movable);
	GetMesh()->SetMaterial(0, DynamicMat);
}

void ABatteryNAssaultCharacter::PossessNewMech()
{
	TArray<AActor*> GameMechs;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMechAICharacter::StaticClass(), GameMechs);

	for (int i = 0; i < GameMechs.Num(); i++)
	{
		AMechAICharacter* Mech = Cast<AMechAICharacter>(GameMechs[i]);

		if (Mech)
		{
			if (Mech->TeamID == TeamID &&!Mech->Controlled)
			{
				FollowCamera->Deactivate();
				Mech->FollowCamera->Activate(true);
				Controller->Possess(Mech);
				Mech->Controlled = true;
				Mech->CharacterMovement->bOrientRotationToMovement = false;
				Destroy();
				return;
			}
		}
	}

	//RootComponent = FollowCamera;
	for (int i = 0; i < RootComponent->GetNumChildrenComponents(); i++)
	{
		if(RootComponent->GetChildComponent(i) != FollowCamera)
			RootComponent->GetChildComponent(i)->DestroyComponent();
	}
	Mesh->DestroyComponent();
	Turret->DestroyComponent();
	FollowCamera->SetRelativeLocationAndRotation(FVector(0, 0, 500), FRotator(-45, 0, 0));;

	DeathState = true;


}