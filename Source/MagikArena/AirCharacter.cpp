// Fill out your copyright notice in the Description page of Project Settings.


#include "AirCharacter.h"
#include "Engine/World.h"

//DASH
void AAirCharacter::MovementAbilityImplementation()
{
	ServerPushBack(GetActorRotation().Vector() * DashStrength);
}
