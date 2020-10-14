// Fill out your copyright notice in the Description page of Project Settings.


#include "SpellWave.h"

#include "BaseCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"

ASpellWave::ASpellWave()
{
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    Mesh->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);
}

void ASpellWave::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if(!bIsAtDestination)
    {
        FVector NewLocation = FMath::VInterpConstantTo(GetActorLocation(), Destination, DeltaTime, Speed);
        SetActorLocation(NewLocation);
        if(NewLocation == Destination)
        {
            bIsAtDestination = true;
        }
    }
    else
    {
        FVector NewLocation = FMath::VInterpConstantTo(GetActorLocation(), DisappearDestination, DeltaTime, DisappearSpeed);
        SetActorLocation(NewLocation);
        if(NewLocation == DisappearDestination)
        {
            Destroy();
        }
    }
}

void ASpellWave::BeginPlay()
{
    Super::BeginPlay();
    OnActorBeginOverlap.AddDynamic(this, &ASpellWave::OnWaveBeginOverlap);
    Destination = GetActorLocation() + GetActorRotation().Vector() * TravelDistance;
    DisappearDestination = Destination;
    //When the waves goes underground it just begins to look smaller untill it disappears
    DisappearDestination.Z -= 1000.0f;
}

void ASpellWave::OnWaveBeginOverlap(AActor* OverpalledActor, AActor* OtherActor)
{
    if(ABaseCharacter* Character = Cast<ABaseCharacter>(OtherActor))
    {
        FVector DirectionToActor = OtherActor->GetActorLocation() - GetActorLocation();
        DirectionToActor.Normalize();
        //The dot product of 2 unit vectors is equal to the cosine of the angle between those 2 vectors
        //so this is just checking if the angle at witch the player was hit is the same as 
        //the activation angle. This is to avoid pushing player back if he entered the wave from the back/side
        if(FVector::DotProduct(DirectionToActor, GetActorForwardVector()) >= FMath::Cos(ActivationAngleDegrees))
        {
            Character->PushBack(GetActorForwardVector() * PushBackStrength);
        }
    }
}
