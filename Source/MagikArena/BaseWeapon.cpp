// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeapon.h"
#include "BaseCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"

ABaseWeapon::ABaseWeapon()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryActorTick.bCanEverTick = false;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	Mesh->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);
}

void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	//Box extent is multiplied by 2 because it gives distance to the center
	Range = Mesh->CalcBounds(FTransform()).BoxExtent.Z * 2.0f;
}

void ABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseWeapon::Attack()
{
	FVector StartLocation = GetOwner()->GetActorLocation();
	FVector LookingDirection = GetOwner()->GetActorRotation().Vector();
	float PlayerReach = Cast<ABaseCharacter>(GetOwner())->GetPlayerReach();
	FVector EndLocation = StartLocation + LookingDirection * (Range + PlayerReach);
	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	CollisionParams.AddIgnoredActor(GetOwner());
	GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECollisionChannel::ECC_Pawn, CollisionParams);
	if (ABaseCharacter* CharacterHit = Cast<ABaseCharacter>(HitResult.GetActor()))
	{
		CharacterHit->TakeDamage(Damage, FDamageEvent(), this->GetInstigatorController(), this);
	}
}

