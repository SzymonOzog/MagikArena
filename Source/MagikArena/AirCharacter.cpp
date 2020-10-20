// Fill out your copyright notice in the Description page of Project Settings.


#include "AirCharacter.h"
#include "Engine/World.h"

//DASH
void AAirCharacter::ServerCastMovementAbility_Implementation()
{
	ServerPushBack(GetActorRotation().Vector() * DashStrength);
}
