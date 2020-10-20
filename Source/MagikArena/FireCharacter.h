// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "FireCharacter.generated.h"

/**
 * 
 */
UCLASS()
class MAGIKARENA_API AFireCharacter : public ABaseCharacter
{
	GENERATED_BODY()
	//ROCKET JUMP
	virtual void ServerCastMovementAbility_Implementation() override;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AExplosion> RocketJumpExplosionClass;
	UPROPERTY(EditAnywhere)
	float RocketJumpForwardDistance = 100.0f;
	UPROPERTY(EditAnywhere)
	float RocketJumpHeight = 1000.0f;
};
