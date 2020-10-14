// Fill out your copyright notice in the Description page of Project Settings.


#include "AirMissile.h"
#include "EngineGlobals.h"
#include "PushComponent.h"
#include "BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"

void AAirMissile::applySpecialEffect(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	if (ABaseCharacter* Character = Cast<ABaseCharacter>(OtherActor))
	{
		if (!Character->IsPlayerDead())
		{
			FVector EnemyLocation = OtherActor->GetActorLocation();
			FVector PlayerLocation = SelfActor->GetActorLocation();
			FVector MoveAmount = calculateMoveAmount(PlayerLocation, EnemyLocation);
			Character->PushBack(MoveAmount);
		}
	}
}

FVector AAirMissile::calculateMoveAmount(const FVector& PlayerLocation, const FVector& EnemyLocation) const
{
	FVector MissileToPlayer = EnemyLocation - PlayerLocation;
	MissileToPlayer.Normalize();
	FVector MissileRotation = GetActorRotation().Vector();
	//Added missile to plater vector with missile rotation vector so the 
	//Push Back direction would depend on the location it hit the player
	//and the direction the missile was going(Rotation follows Velocity is true)
	FVector MoveAmount = MissileToPlayer + MissileRotation;
	MoveAmount *= PushStrength;
	return MoveAmount;
}
