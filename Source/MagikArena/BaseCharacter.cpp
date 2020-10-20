// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseCharacter.h"
#include "BaseMissile.h"
#include "BaseSpell.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "Engine/DecalActor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "TimerManager.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	SpringArm->bUsePawnControlRotation = true;
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->AttachToComponent(SpringArm, FAttachmentTransformRules::KeepRelativeTransform);
	Camera->bUsePawnControlRotation = false;
	
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;
	SetupCharacterMovement();
	SetupMissileSpawnParams();
	FrictionFactor = GetCharacterMovement()->BrakingFrictionFactor;
}

void ABaseCharacter::SetupCharacterMovement()
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->AirControl = this->AirControl;
	GetCharacterMovement()->MaxWalkSpeed *= Speed;
	GetCharacterMovement()->JumpZVelocity = JumpHeight;
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bIsAimingSpell && SpellIndicator)
	{
		ServerRotateToControllerYaw();
		RotateSpellIndicator(DeltaTime);
		SetSpellIndicatorLocation();
	}
}

void ABaseCharacter::RotateSpellIndicator(float DeltaTime)
{
	SpellIndicatorRotator.Yaw += 50.0f * DeltaTime;
	FRotator PlayerRollRotation = FRotator::ZeroRotator;
	PlayerRollRotation.Yaw = GetActorRotation().Yaw;
	SpellIndicator->SetActorRotation(SpellIndicatorRotator + PlayerRollRotation);
}

void ABaseCharacter::SetSpellIndicatorLocation() const
{
	FVector PlayerViewLocation;
	FRotator PlayerViewRotation;
	Controller->GetPlayerViewPoint(PlayerViewLocation, PlayerViewRotation);
	float SpellRange = 	SpellClass->GetDefaultObject<ABaseSpell>()->GetCastingRange();
	FVector LineTraceEnd = PlayerViewLocation + PlayerViewRotation.Vector() * SpellRange;
	FHitResult Hit;
	if (GetWorld()->LineTraceSingleByChannel(Hit, PlayerViewLocation, LineTraceEnd, ECollisionChannel::ECC_Visibility))
	{
		SpellIndicator->SetActorLocation(Hit.Location);
	}
	else// Line trace from the end to the ground and draw the Spell Indicator there
		{
		FVector LineTraceEndToGround = LineTraceEnd;
		LineTraceEndToGround.Z -= SpellRange;
		if (GetWorld()->LineTraceSingleByChannel(Hit, LineTraceEnd, LineTraceEndToGround, ECollisionChannel::ECC_Visibility))
		{
			SpellIndicator->SetActorLocation(Hit.Location);				
		}
		}
}

float ABaseCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float DamageApplied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	CurrentHealth = FMath::Clamp(CurrentHealth - DamageApplied, 0.0f, MaxHealth);
	if (IsPlayerDead())
	{
		HandleDeath();
	}
	return DamageAmount;
}


void ABaseCharacter::HandleDeath()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DetachFromControllerPendingDestroy();
	//Turn on meshes physics to make it fall on the ground
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionProfileName(TEXT("BlockAll"));
}


// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ABaseCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ABaseCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &APawn::AddControllerYawInput);

	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Attack"), IE_Released, this, &ABaseCharacter::StopAttacking);
	PlayerInputComponent->BindAction(TEXT("Attack"), IE_Pressed, this, &ABaseCharacter::ServerAttack);
    PlayerInputComponent->BindAction(TEXT("SpecialMovement"), IE_Released, this, &ABaseCharacter::MovementAbility);

}

void ABaseCharacter::MoveForward(float AxisValue)
{
	if (Controller && AxisValue)
	{
		const FRotator& Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector& Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, AxisValue * Speed);
	}
}

void ABaseCharacter::MoveRight(float AxisValue)
{
	if (Controller && AxisValue)
	{
		const FRotator& Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector& Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, AxisValue * Speed);
	}
}

void ABaseCharacter::ServerAttack_Implementation()
{
	//attacks are made with respect to controller rotation so
	//we are rotating to prevent attacking with our back
	ServerRotateToControllerYaw();
	bIsAttacking = true;
	if (GetWorld()->GetTimeSeconds() - MissileLastCast < MissileCooldown)
	{
		return;
	}
	MissileLastCast = GetWorld()->GetTimeSeconds();
	if (MissileClass)
	{
		GetWorld()->SpawnActor<ABaseMissile>(MissileClass, CalculateMissileSpawnTransform(), MissileSpawnParams);
	}
}

void ABaseCharacter::ServerRotateToControllerYaw_Implementation()
{
	FRotator playerRotation = GetActorRotation();
	FRotator controllerRotation = GetControlRotation();
	playerRotation.Yaw = controllerRotation.Yaw;
	SetActorRotation(playerRotation);
}

FTransform ABaseCharacter::CalculateMissileSpawnTransform() const
{
	FTransform MissileSpawnTransform;
	FRotator ControllerRotation = GetControlRotation();
	MissileSpawnTransform.SetLocation(ControllerRotation.Vector() * MissileSpawnDistance + GetActorLocation());
	MissileSpawnTransform.SetRotation(ControllerRotation.Quaternion());
	return MissileSpawnTransform;
}

void ABaseCharacter::StopAttacking()
{
	bIsAttacking = false;
}

void ABaseCharacter::SetupMissileSpawnParams()
{
	MissileSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	MissileSpawnParams.bNoFail = true;
	MissileSpawnParams.Owner = this;
	MissileSpawnParams.Instigator = this;
}

void ABaseCharacter::AimSpell()
{
	if (SpellIndicatorClass)
	{
		bIsAimingSpell = true;
		SpellIndicator = GetWorld()->SpawnActor<ADecalActor>(SpellIndicatorClass, GetActorLocation(), FRotator::ZeroRotator);
		SpellIndicatorRotator = SpellIndicator->GetActorRotation();
	}
}

void ABaseCharacter::CastSpell()
{
	bIsAimingSpell = false;
	if (SpellIndicator)
	{
		GetWorld()->SpawnActor<ABaseSpell>(SpellClass, SpellIndicator->GetActorLocation(), GetActorRotation());
		SpellIndicator->Destroy();
	}
	 
}

void ABaseCharacter::MovementAbility()
{
	if(GetWorld()->GetTimeSeconds() - MovementAbilityCooldown >= MovementAbilityLastCast)
	{
		MovementAbilityLastCast = GetWorld()->GetTimeSeconds();
		MovementAbilityImplementation();
	}
}

void ABaseCharacter::PushBack(FVector PushDirection, float Duration)
{
	PushDirection *= GetMesh()->GetMass();
	GetCharacterMovement()->BrakingFrictionFactor = 0.0f;
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->AddImpulse(PushDirection, true);
	FTimerHandle Handle;
	GetWorldTimerManager().SetTimer(Handle, this, &ABaseCharacter::StopPushBack, Duration, false);
}

void ABaseCharacter::StopPushBack() const
{
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->BrakingFrictionFactor = FrictionFactor;
}

void ABaseCharacter::HitSpikes()
{
	SpikeHitTime = GetWorld()->GetTimeSeconds();
}

