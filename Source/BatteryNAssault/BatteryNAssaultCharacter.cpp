// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "BatteryNAssault.h"
#include "BatteryNAssaultCharacter.h"
#include "MechAICharacter.h"
#include "PowerUp.h"
#include "BehaviorTree/BehaviorTree.h"

//////////////////////////////////////////////////////////////////////////
// ABatteryNAssaultCharacter

ABatteryNAssaultCharacter::ABatteryNAssaultCharacter()
{
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinder<UClass> MachineGun;
		ConstructorHelpers::FObjectFinder<UClass> Shotgun;
		ConstructorHelpers::FObjectFinder<UClass> GrenadeLauncher;

		//FConstructorStatics() : MachineGun(TEXT("Class'/Game/Weapon/ProjectileWeapons/MachineGun.MachineGun_C'")) {}
		FConstructorStatics() : MachineGun(TEXT("Class'/Game/Weapon/ProjectileWeapons/MachineGun.MachineGun_C'")),
								Shotgun(TEXT("Class'/Game/Weapon/ProjectileWeapons/RocketLauncher/RocketLauncher.RocketLauncher_C'")),
								GrenadeLauncher(TEXT("Class'/Game/Weapon/ProjectileWeapons/GrenadeLauncher/GrenadeLauncher.GrenadeLauncher_C'")) {}
	};

	static FConstructorStatics ConstructorStatics;


	if (ConstructorStatics.MachineGun.Object)
	{
		Gun = Cast<UClass>(ConstructorStatics.MachineGun.Object);
	}

	//Temp->K2_SetWorldRotation(FollowCamera.)

	WeaponInd = 0; // Index of currently equipped weapon
	EnergyCostPerSecond = 0.5f;
	MaxEnergy = 100.f;
	Energy = MaxEnergy;

	MaxHealth = 100.f;
	Health = MaxHealth;
	DeathState = false;

	bInEnemySight = false;
	bGameIsOver = false;
	TimeUntilOver = 2.0f;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	OnActorHit.AddDynamic(this, &ABatteryNAssaultCharacter::OnCollision);
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
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relativHitComponent->AttachTo(RootComponent);


	static ConstructorHelpers::FObjectFinder<UParticleSystem> Damage(TEXT("ParticleSystem'/Game/StarterContent/Particles/DestoryP.DestoryP'"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> Explosion(TEXT("ParticleSystem'/Game/StarterContent/Particles/FireP.FireP'"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> OnHit(TEXT("ParticleSystem'/Game/StarterContent/Particles/HitP.HitP'"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> GunPart(TEXT("ParticleSystem'/Game/StarterContent/Particles/P_Fire.P_Fire'"));

	DamageParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("DamageParticle"));
	DestroyComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("DestroyComponent"));
	OnHitComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("OnHitComponent"));
	GunParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("GunParticle"));


	DamageParticle->bAutoActivate = false;
	DestroyComponent->bAutoActivate = false;
	OnHitComponent->bAutoActivate = false;
	GunParticle->bAutoActivate = false;

	GunParticle->Template = GunPart.Object;
	DamageParticle->Template = Damage.Object;
	DestroyComponent->Template = Explosion.Object;
	OnHitComponent->Template = OnHit.Object;

	GunParticle->AttachTo(Turret, "FirePoint", EAttachLocation::SnapToTarget);
	DamageParticle->AttachTo(RootComponent);
	DestroyComponent->AttachTo(RootComponent);
	OnHitComponent->AttachTo(RootComponent);

	ConstructorHelpers::FObjectFinder<USoundCue> EngineSoundHolder(TEXT("SoundCue'/Game/MAARS/Sounds/S_Engine-loop_Cue.S_Engine-loop_Cue'"));
	EngineSound = EngineSoundHolder.Object;
	AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComp"));
	AudioComp->AttachTo(RootComponent);
	AudioComp->bAutoActivate = false;

	TeamID = 0;
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	//Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//DOREPLIFETIME(ABatteryNAssaultCharacter, TowerRotation);
}

void ABatteryNAssaultCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate to everyone
	DOREPLIFETIME(ABatteryNAssaultCharacter, TowerRotation);
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

	if (EngineSound)
	{

		AudioComp = UGameplayStatics::SpawnSoundAttached(EngineSound, this->GetRootComponent());
	}
}

void ABatteryNAssaultCharacter::DeathFunc()
{
	if (DeathState) return;

	DeathState = true;
	Health = 0;
	Energy = 0;
}

void ABatteryNAssaultCharacter::ChangeWeapon(int32 ind)
{
	if (WeaponInd == ind) return;

	FString Message = FString::Printf(TEXT("Changed Weapon ID: %d"), ind);
	GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::White, Message);
	
	if (ind == 0) // Machine Gun
	{
		Gun = GunTypes[0];
	}
	else if (ind == 1) // Shotgun
	{
		Gun = GunTypes[1];
	}
	else if (ind == 2) // Rocket Launcher
	{
		Gun = GunTypes[2];
	}

	Weapon->Destroy();

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = this;
	SpawnParameters.Instigator = this;
	AWeapon *Spawn = GetWorld()->SpawnActor<AWeapon>(Gun, SpawnParameters);
	if (Spawn)
	{
		Spawn->AttachRootComponentTo(Turret, "FirePoint", EAttachLocation::SnapToTarget);
		Weapon = Spawn;
	}

	WeaponInd = ind;
}

void ABatteryNAssaultCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bGameIsOver)
	{
		TimeUntilOver -= DeltaTime;
		if (TimeUntilOver <= 0)
		{
			UGameplayStatics::OpenLevel(GetWorld(), "MainMenu");
		}
	}
	if (DeathState) 
		return;
	if (Energy > 0)
	{
		Energy -= EnergyCostPerSecond * DeltaTime;
	}

	if (Health <= 0)
	{
		DestroyComponent->ActivateSystem();
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

	//if (Role == ROLE_Authority)
	if( HasAuthority() )
	{
		if (TowerRotation != BaseRotation)
		{
			FRotator currentBaseRotation = FMath::RInterpTo(TowerRotation, BaseRotation, GetWorld()->GetDeltaSeconds(), 3.0f);
			TowerRotation = currentBaseRotation;
		}
	}

	/*
	if (TowerRotation != BaseRotation)
	{
		FRotator currentBaseRotation = FMath::RInterpTo(TowerRotation, BaseRotation, GetWorld()->GetDeltaSeconds(), 3.0f);
		TowerRotation = currentBaseRotation;
	}
	*/

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

	if (HasAuthority())
	{
		FString Message = "Move Riight";
//		FString Message = FString::Printf(TEXT("Energy: %.2f"), Energy);
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::White, Message);
	}
	/*
	else
	{

		FString Message = FString::Printf( TEXT("%.2f"), Value);
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::White, Message);
	}*/

	//AddControllerYawInput(Value * GetWorld()->GetDeltaSeconds());
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
	GunParticle->ActivateSystem();
}

void ABatteryNAssaultCharacter::StopFire()
{
	Weapon->EndAttack();
	GunParticle->DeactivateSystem();
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

	if (Health < 75 )
	{
		this->DamageParticle->ActivateSystem();
	}
	if (Health < 50)
	{
		this->DestroyComponent->ActivateSystem();
	}
	else if (Health > 50)
	{
		this->DestroyComponent->DeactivateSystem();
	}
	else if (Health > 75)
	{
		this->DamageParticle->DeactivateSystem();
	}


	ABatteryNAssaultCharacter* damageDealer = Cast<ABatteryNAssaultCharacter>(DamageCauser);

	if (damageDealer)
	{
		if (damageDealer->TeamID != this->TeamID)
		{	
			this->OnHitComponent->ActivateSystem();
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

void ABatteryNAssaultCharacter::OnCollision(AActor* SelfActor, AActor *OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	ABatteryNAssaultCharacter* other = Cast<ABatteryNAssaultCharacter>(OtherActor);
	if (other)
	{
		FVector Direction = OtherActor->GetActorLocation() - SelfActor->GetActorLocation();
		Direction.Normalize();

		CharacterMovement->Velocity -= (Direction * 2000) + FVector::UpVector * 1000;
		AMechAICharacter* AI = Cast<AMechAICharacter>(other);

		UGameplayStatics::ApplyDamage(
			OtherActor,
			1,
			GetController(),
			this,
			UDamageType::StaticClass());
		if (AI)
		{
			AI->IsStunned = true;
			AI->StunTime = 0;
		}
	}
}