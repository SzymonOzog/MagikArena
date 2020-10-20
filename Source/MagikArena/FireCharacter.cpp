// Fill out your copyright notice in the Description page of Project Settings.


#include "FireCharacter.h"


#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Explosion.h"
//ROCKET JUMP
void AFireCharacter::ServerCastMovementAbility_Implementation()
{
    if(!GetCharacterMovement()->IsFalling())
    	{
    		if(RocketJumpExplosionClass)
    		{
    			GetWorld()->SpawnActor<AExplosion>(RocketJumpExplosionClass, GetActorLocation(), GetActorRotation());
    		}
    		FVector JumpDirection = GetActorForwardVector() * RocketJumpForwardDistance;
    		JumpDirection.Z = RocketJumpHeight;
    	
    		GetCharacterMovement()->AddImpulse(JumpDirection, true);
    		JumpCurrentCount--;
    	}
}
