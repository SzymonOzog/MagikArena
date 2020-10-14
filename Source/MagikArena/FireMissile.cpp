// Fill out your copyright notice in the Description page of Project Settings.


#include "FireMissile.h"

#include "Engine/World.h"
#include "Explosion.h"

void AFireMissile::applySpecialEffect(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	if (ExplosionClass)
	{
		GetWorld()->SpawnActor<AExplosion>(ExplosionClass, Hit.Location, FRotator::ZeroRotator);
	}
}