// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseMissile.h"
#include "BaseCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ABaseMissile::ABaseMissile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MissileMesh = CreateDefaultSubobject<UStaticMeshComponent>("MissileMesh");
	SetRootComponent(MissileMesh);

	MissileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("MissileMovement");
	MissileMovement->InitialSpeed = MissileSpeed;
	MissileMovement->MaxSpeed = MissileSpeed;
	MissileMovement->ProjectileGravityScale = GravityScale;
	MissileMovement->bRotationFollowsVelocity = true;

	OnActorHit.AddDynamic(this, &ABaseMissile::OnMissileHit);
}

// Called when the game starts or when spawned
void ABaseMissile::BeginPlay()
{
	Super::BeginPlay();
	OwningPlayer = Cast<ABaseCharacter>(GetOwner());
}

// Called every frame
void ABaseMissile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//if player is holding LMB change velocity according to his location
	if (OwningPlayer && OwningPlayer->IsCharacterAttacking())
	{
		FVector LookingDirection = OwningPlayer->GetControlRotation().Vector();
		//OwningPlayers looking direction is in global space, we need to translate it to Missiles local space
		FVector TranslatedLookingDirection = UKismetMathLibrary::InverseTransformDirection(GetTransform(), LookingDirection);
		MissileMovement->SetVelocityInLocalSpace(TranslatedLookingDirection * MissileSpeed);
	}
}

void ABaseMissile::OnMissileHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	applySpecialEffect(SelfActor, OtherActor, NormalImpulse, Hit);
	if (ABaseCharacter* Character = Cast<ABaseCharacter>(OtherActor))
	{
		Character->TakeDamage(Damage, FDamageEvent(), GetInstigator()->GetController(), GetOwner());
	}
	Destroy();
}

