// Fill out your copyright notice in the Description page of Project Settings.


#include "SpellMeteor.h"
#include "Components/SkeletalMeshComponent.h"
#include "BaseCharacter.h"
#include "Explosion.h"
#include "DestructibleComponent.h"

ASpellMeteor::ASpellMeteor()
{
    PrimaryActorTick.bCanEverTick = true;
    DestructibleComponent = CreateDefaultSubobject<UDestructibleComponent>(TEXT("Destructible Mesh"));
    DestructibleComponent->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);
}

void ASpellMeteor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    FVector MeshLocation = FMath::VInterpConstantTo(DestructibleComponent->GetRelativeLocation(), FVector::ZeroVector, DeltaTime, Speed);
    DestructibleComponent->SetRelativeLocation(MeshLocation, true);
}

void ASpellMeteor::BeginPlay()
{
    Super::BeginPlay();
	OnActorHit.AddDynamic(this, &ASpellMeteor::OnMeteorHit);
    DestructibleComponent->SetRelativeLocation(FVector(0.0f,0.0f, SpawnHeight));
    DestructibleComponent->SetWorldScale3D(FVector(Size)); //TODO make it work
}

void ASpellMeteor::OnMeteorHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
    if(ABaseCharacter* Character = Cast<ABaseCharacter>(OtherActor))
    {
        Character->TakeDamage(Damage, FDamageEvent(), nullptr, this);
    }
    DestructibleComponent->ApplyDamage(1.0f, DestructibleComponent->GetComponentLocation(), NormalImpulse, 100.0f);
    OnActorHit.RemoveDynamic(this, &ASpellMeteor::OnMeteorHit);
}
