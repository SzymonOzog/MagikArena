// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "EarthCharacter.generated.h"

/**
 * 
 */
UCLASS()
class MAGIKARENA_API AEarthCharacter : public ABaseCharacter
{
	GENERATED_BODY()
public:
	void SetupUndergroundInput(UInputComponent* PlayerInputComponent);	
private:
	//HIDE UNDERGROUND
	virtual void MovementAbilityImplementation() override;
	UPROPERTY(EditAnywhere)
	float HideUndergroundDuration = 1.5f;
};
