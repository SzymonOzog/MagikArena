// Fill out your copyright notice in the Description page of Project Settings.


#include "Lava.h"
#include "BaseCharacter.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
ALava::ALava()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);
}

void ALava::BeginPlay()
{
	Super::BeginPlay();
	OnActorBeginOverlap.AddDynamic(this, &ALava::OnLavaBeginOverlap);
}

void ALava::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALava::OnLavaBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (ABaseCharacter* Character = Cast<ABaseCharacter>(OtherActor))
	{
		GetWorldTimerManager().SetTimer(TimerHandle, this, &ALava::DealLavaDamage, 1.0f, true, 1.0f);
	}

}

void ALava::DealLavaDamage()
{
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors);
	for (AActor* OverlappedActor : OverlappingActors)
	{
		if (ABaseCharacter* Character = Cast<ABaseCharacter>(OverlappedActor))
		{
			Character->TakeDamage(DamagePerSecond, FDamageEvent(), nullptr, this);
			UGameplayStatics::SpawnEmitterAttached(Fire, Character->GetMesh(), TEXT("spine_01"));
		}
	}
}

