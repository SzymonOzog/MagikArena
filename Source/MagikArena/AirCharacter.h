// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "AirCharacter.generated.h"

/**
 * 
 */
UCLASS()
class MAGIKARENA_API AAirCharacter : public ABaseCharacter
{
	GENERATED_BODY()
	//DASH
	virtual void ServerCastMovementAbility_Implementation() override;
	UPROPERTY(EditDefaultsOnly)
	float DashStrength = 300.0f;
};
