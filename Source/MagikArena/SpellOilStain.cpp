// Fill out your copyright notice in the Description page of Project Settings.


#include "SpellOilStain.h"

#include <ratio>



#include "AirMissile.h"
#include "FireMissile.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/World.h"
#include "Engine/DecalActor.h"
#include "BaseCharacter.h"
#include "TimerManager.h"

ASpellOilStain::ASpellOilStain()
{
}

void ASpellOilStain::Tick(float DeltaTime)
{
}

void ASpellOilStain::BeginPlay()
{
    Super::BeginPlay();
    OnActorBeginOverlap.AddDynamic(this, &ASpellOilStain::OnOilBeginOverlap);
    OnActorEndOverlap.AddDynamic(this, &ASpellOilStain::OnOilEndOverlap);
    Decal = GetWorld()->SpawnActor<ADecalActor>(OilStainClass, GetActorLocation(), GetActorRotation());
}

void ASpellOilStain::OnOilBeginOverlap(AActor* OverpalledActor, AActor* OtherActor)
{
    if(ACharacter* Character = Cast<ACharacter>(OtherActor))
    {
        Character->GetCharacterMovement()->MaxWalkSpeed /= SlowStrength;
    }
    if(Cast <AFireMissile>(OtherActor))
    {
        Decal->Destroy();
        Decal = GetWorld()->SpawnActor<ADecalActor>(BurningOilStainClass, GetActorLocation(), GetActorRotation());
        GetWorldTimerManager().SetTimer(BurnTimerHandle, this, &ASpellOilStain::Burn, 1.0f, true, 0.5f);
    }
}

void ASpellOilStain::OnOilEndOverlap(AActor* OverpalledActor, AActor* OtherActor)
{
    if(ACharacter* Character = Cast<ACharacter>(OtherActor))
    {
        Character->GetCharacterMovement()->MaxWalkSpeed *= SlowStrength;
    }
    if(Cast <AAirMissile>(OtherActor))
    {
        Decal->Destroy();
        Decal = GetWorld()->SpawnActor<ADecalActor>(OilStainClass, GetActorLocation(), GetActorRotation());
        GetWorldTimerManager().ClearTimer(BurnTimerHandle);
    }
}

void ASpellOilStain::Burn()
{
    TArray<AActor*> OverlappingActors;
    GetOverlappingActors(OverlappingActors);
    for (AActor* OverlappedActor : OverlappingActors)
    {
        if(ABaseCharacter* Character = Cast<ABaseCharacter>(OverlappedActor))
        {
            Character->TakeDamage(Damage, FDamageEvent(), nullptr, this);
        }
    }
}
