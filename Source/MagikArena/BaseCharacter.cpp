// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseCharacter.h"
#include "BaseMissile.h"
#include "BaseSpell.h"
#include "PlayerInfoWidget.h"
#include "MagikArenaGameModeBase.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Engine/World.h"
#include "Engine/DecalActor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "TimerManager.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	SpringArm->SetRelativeLocation(FVector(0.0f, 60.0f, 100.0f));
	SpringArm->bUsePawnControlRotation = true;
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->AttachToComponent(SpringArm, FAttachmentTransformRules::KeepRelativeTransform);
	Camera->bUsePawnControlRotation = false;

	HealthBarName = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealhBarName"));
	HealthBarName->SetWidgetSpace(EWidgetSpace::Screen);
	HealthBarName->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	HealthBarName->SetRelativeLocation(FVector(0.0f,0.0f,110.0f));

	Crosshair = CreateDefaultSubobject<UWidgetComponent>(TEXT("Crosshair"));
	Crosshair->SetWidgetSpace(EWidgetSpace::Screen);
	Crosshair->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	Crosshair->SetVisibility(false);
}	

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;
	SetupCharacterMovement();
	SetupMissileSpawnParams();
	FrictionFactor = GetCharacterMovement()->BrakingFrictionFactor;
	if(IsLocallyControlled())
	{
		Crosshair->SetVisibility(true);
		//ClientShowCrosshairWidget();
	}
	if(HealthBarClass)
	{
		CreateHealthBar();
	}
	if(InterfaceClass)
	{
		CreateWidget<UUserWidget>(GetWorld(), InterfaceClass)->AddToViewport();
	}
}

void ABaseCharacter::CreateHealthBar()
{
	UPlayerInfoWidget* HealthBar = CreateWidget<UPlayerInfoWidget>(GetWorld(), HealthBarClass);
	HealthBar->RepresentedCharacter = this;
	HealthBarName->SetWidget(HealthBar);
	HealthBarName->SetVisibility(true);
	HealthBarName->RegisterComponent();
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
		RotateSpellIndicator(DeltaTime);
		SetSpellIndicatorLocation();
	}
	if(IsLocallyControlled())
	{
		Crosshair->SetVisibility(true);
		DrawCrosshair();
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
	FTransform MissileSpawnTransform = CalculateMissileSpawnTransform();
	FVector MissileSpawnLocation = MissileSpawnTransform.GetLocation();
	float SpellRange = 	SpellClass->GetDefaultObject<ABaseSpell>()->GetCastingRange();
	FVector LineTraceEnd = MissileSpawnLocation + MissileSpawnTransform.GetRotation().Vector() * SpellRange;
	FHitResult Hit;
	if (GetWorld()->LineTraceSingleByChannel(Hit, MissileSpawnLocation, LineTraceEnd, ECollisionChannel::ECC_Visibility))
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
		if(APlayerController* PlayerController = Cast<APlayerController>(Controller))
		{
	        FTimerHandle Handle;
	        GetWorldTimerManager().SetTimer(Handle, [this, PlayerController]()
	        {
	        Cast<AMagikArenaGameModeBase>(GetWorld()->GetAuthGameMode())->Respawn(PlayerController);
	        }, RespawnTime, false);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Not a player controller"))
		}
		MulticastHandleDeath();
	}
	return DamageAmount;
}


void ABaseCharacter::MulticastHandleDeath_Implementation()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DetachFromControllerPendingDestroy();
	//Turn on meshes physics to make it fall on the ground
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionProfileName(TEXT("BlockAll"));
	FTimerHandle Handle;
	GetWorldTimerManager().SetTimer(Handle, [this](){Destroy();}, RespawnTime, false);
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
	PlayerInputComponent->BindAction(TEXT("Attack"), IE_Pressed, this, &ABaseCharacter::ServerAttack);
	PlayerInputComponent->BindAction(TEXT("Attack"), IE_Released, this, &ABaseCharacter::ServerStopAttacking);
    PlayerInputComponent->BindAction(TEXT("SpecialMovement"), IE_Released, this, &ABaseCharacter::MovementAbility);
	PlayerInputComponent->BindAction(TEXT("CastSpell"), IE_Pressed, this, &ABaseCharacter::AimSpell);
	PlayerInputComponent->BindAction(TEXT("CastSpell"), IE_Released, this, &ABaseCharacter::ClientCastSpell);

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
	MulticastRotateToControllerYaw();
	bIsAttacking = true;
	if (GetWorld()->GetTimeSeconds() - MissileLastCast < MissileCooldown)
	{
		return;
	}
	MissileLastCast = GetWorld()->GetTimeSeconds();
	if (MissileClass)
	{
		CurrentMissile = GetWorld()->SpawnActor<ABaseMissile>(MissileClass, CalculateMissileSpawnTransform(), MissileSpawnParams);
	}
}

void ABaseCharacter::MulticastRotateToControllerYaw_Implementation()
{
	FRotator PlayerRotation = GetActorRotation();
	FRotator ControllerRotation = GetControlRotation();
	PlayerRotation.Yaw = ControllerRotation.Yaw;
	FString DebugText = TEXT("Called Rotate, control rotation is") + ControllerRotation.ToString();
	UKismetSystemLibrary::PrintString(this, DebugText, false, true,  FLinearColor::Red); 
	SetActorRotation(PlayerRotation);
}

FTransform ABaseCharacter::CalculateMissileSpawnTransform() const
{
	FTransform MissileSpawnTransform;
	FRotator ControllerRotation = GetControlRotation();
	MissileSpawnTransform.SetLocation(ControllerRotation.Vector() * MissileSpawnDistance + GetActorLocation());
	MissileSpawnTransform.SetRotation(ControllerRotation.Quaternion());
	return MissileSpawnTransform;
}

void ABaseCharacter::ServerStopAttacking_Implementation()
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
	float SpellCooldown = SpellClass->GetDefaultObject<ABaseSpell>()->GetCooldown();
	if(GetWorld()->GetTimeSeconds() - SpellCooldown >= SpellLastCast)
	{
		if (SpellIndicatorClass)
		{
			bIsAimingSpell = true;
			SpellIndicator = GetWorld()->SpawnActor<ADecalActor>(SpellIndicatorClass, GetActorLocation(), FRotator::ZeroRotator);
			SpellIndicatorRotator = SpellIndicator->GetActorRotation();
		}
	}
}

void ABaseCharacter::ClientCastSpell_Implementation()
{
	bIsAimingSpell = false;
	if (SpellIndicator)
	{
		SpellLastCast = GetWorld()->GetTimeSeconds();
		ServerCastSpell(SpellIndicator->GetActorLocation());
		SpellIndicator->Destroy();
		SpellIndicator = nullptr;
	}
}

void ABaseCharacter::ServerCastSpell_Implementation(FVector SpawnLocation)
{
	GetWorld()->SpawnActor<ABaseSpell>(SpellClass, SpawnLocation, GetActorRotation());
}

void ABaseCharacter::MovementAbility()
{
	if(GetWorld()->GetTimeSeconds() - MovementAbilityCooldown >= MovementAbilityLastCast)
	{
		MovementAbilityLastCast = GetWorld()->GetTimeSeconds();
		ServerCastMovementAbility();
	}
}

void ABaseCharacter::DrawCrosshair()
{
	FTransform LineTraceTransform;
	TArray<AActor*> ActorsToIgnore;
	if(IsValid(CurrentMissile))
	{
		LineTraceTransform = CurrentMissile->GetTransform();
		ActorsToIgnore.Add(CurrentMissile);
		UE_LOG(LogTemp, Warning, TEXT("MissileIsValid"))
	}
	else
	{
		LineTraceTransform = CalculateMissileSpawnTransform();
	}
	FVector LineTraceStart = LineTraceTransform.GetLocation();
	FVector LineTraceEnd = LineTraceStart + LineTraceTransform.GetRotation().Vector() * 10000.0f;
	FHitResult Hit;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	//if(GetWorld()->LineTraceSingleByChannel(Hit, LineTraceStart, LineTraceEnd, ECollisionChannel::ECC_Visibility))
	if(UKismetSystemLibrary::SphereTraceSingle(GetWorld(),LineTraceStart,LineTraceEnd, 50.0f,ETraceTypeQuery::TraceTypeQuery1, false, ActorsToIgnore, EDrawDebugTrace::None, Hit, true))
	{
		Crosshair->SetWorldLocation(Hit.Location);
	}
}

void ABaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABaseCharacter, CurrentHealth);
	DOREPLIFETIME(ABaseCharacter, CurrentMissile);
}

void ABaseCharacter::ServerPushBack_Implementation(FVector PushDirection, float Duration)
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

float ABaseCharacter::GetTimeUntillSpell()
{
	float SpellCooldown = SpellClass->GetDefaultObject<ABaseSpell>()->GetCooldown();
	float TimeUntilSpell = FMath::Clamp(SpellCooldown - GetWorld()->GetTimeSeconds() + SpellLastCast, 0.0f, SpellCooldown);
	return TimeUntilSpell;
}

float ABaseCharacter::GetTimeUntillMovementAbility()
{
	float TimeUntilMovementAbility = FMath::Clamp(MovementAbilityCooldown - GetWorld()->GetTimeSeconds() + MovementAbilityLastCast, 0.0f, MovementAbilityCooldown);
	return TimeUntilMovementAbility;
}
