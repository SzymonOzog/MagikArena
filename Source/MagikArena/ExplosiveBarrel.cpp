// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplosiveBarrel.h"
#include "Explosion.h"
#include "Engine/World.h"
AExplosiveBarrel::AExplosiveBarrel()
{
	PrimaryActorTick.bCanEverTick = false;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
}

void AExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
	OnActorBeginOverlap.AddDynamic(this, &AExplosiveBarrel::OnBarrelOverlap);
}

void AExplosiveBarrel::OnBarrelOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (Cast<AExplosion>(OtherActor))
	{
		if (ExplosionClass)
		{
			Destroy();
			GetWorld()->SpawnActor<AExplosion>(ExplosionClass, GetActorLocation(), FRotator::ZeroRotator);
		}
	}
}



