// Fill out your copyright notice in the Description page of Project Settings.


#include "AirCharacter.h"
#include "Engine/World.h"

void AAirCharacter::MovementAbilityImplementation()
{
	PushBack(GetActorRotation().Vector() * DashStrength);
}
