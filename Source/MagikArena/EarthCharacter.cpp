// Fill out your copyright notice in the Description page of Project Settings.


#include "EarthCharacter.h"



#include "TimerManager.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

void AEarthCharacter::SetupUndergroundInput(UInputComponent* PlayerInputComponent)
{
    PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ABaseCharacter::MoveForward);
    PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ABaseCharacter::MoveRight);
    PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
    PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &APawn::AddControllerYawInput);
}

//HIDE UNDERGROUND
void AEarthCharacter::ServerCastMovementAbility_Implementation()
{
    if(!GetCharacterMovement()->IsFalling())
    {
        SetActorHiddenInGame(true);
        const FName CapsuleCollision = GetCapsuleComponent()->GetCollisionProfileName();
        const FName MeshCollision = GetMesh()->GetCollisionProfileName();
        GetCapsuleComponent()->SetCollisionProfileName(FName("Ghost"));
        GetMesh()->SetCollisionProfileName(FName("Ghost"));
        InputComponent->ClearActionBindings();
        SetupUndergroundInput(InputComponent);

        FTimerHandle Handle;
        GetWorldTimerManager().SetTimer(Handle,
            [this, CapsuleCollision, MeshCollision]()
            {
                GetCapsuleComponent()->SetCollisionProfileName(CapsuleCollision);
                GetMesh()->SetCollisionProfileName(MeshCollision);
                SetActorHiddenInGame(false);
                SetupPlayerInputComponent(InputComponent);
            }, HideUndergroundDuration, false);
    }
}
