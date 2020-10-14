// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseSpell.h"
#include "Engine/World.h"
#include "Components/StaticMeshComponent.h"
// Sets default values
ABaseSpell::ABaseSpell()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
}

void ABaseSpell::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseSpell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

