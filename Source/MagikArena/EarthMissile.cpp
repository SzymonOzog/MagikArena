// Fill out your copyright notice in the Description page of Project Settings.


#include "EarthMissile.h"
#include "BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"
void AEarthMissile::applySpecialEffect(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
    if(ABaseCharacter* Character = Cast<ABaseCharacter>(OtherActor))
    {
        Character->GetCharacterMovement()->MaxWalkSpeed /= SlowStrength;
        FTimerHandle Handle;
        GetWorldTimerManager().SetTimer(Handle,
            [Character, this](){Character->GetCharacterMovement()->MaxWalkSpeed *= SlowStrength;},
            SlowDuration, false);
    }
}
