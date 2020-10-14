// Fill out your copyright notice in the Description page of Project Settings.


#include "Spikes.h"
#include "BaseCharacter.h"
#include "Components/StaticMeshComponent.h"
ASpikes::ASpikes()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);
}

void ASpikes::BeginPlay()
{
	Super::BeginPlay();
	OnActorHit.AddDynamic(this, &ASpikes::OnSpikesHit);
}

void ASpikes::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASpikes::OnSpikesHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	if (ABaseCharacter* Character = Cast<ABaseCharacter>(OtherActor))
	{
		if (GetWorld()->GetTimeSeconds() - Character->GetSpikeHitTime() >= TimeBetweenHits)
		{
			FVector CharacterPositionInSpikesSpace = Character->GetActorLocation() - SelfActor->GetActorLocation();
			FVector SpikesRotationVector = GetActorRotation().Vector();
			CharacterPositionInSpikesSpace.Normalize();
			//The dot product of 2 unit vectors is equal to the cosine of the angle between those 2 vectors
			//so this is just checking if the angle at witch the player has hit the spikes is the same as 
			//the activation angle. This is to avoid damaging the player if he hit the spikes from the back/side
			float DotProduct = FVector::DotProduct(SpikesRotationVector, CharacterPositionInSpikesSpace);
			if (DotProduct >= FMath::Cos(ActivationAngleDegrees))
			{
				Character->HitSpikes();
				Character->TakeDamage(Damage, FDamageEvent(), nullptr, this);
				//Push the player back 
				if (!Character->IsPlayerDead())
				{
					Character->PushBack(CharacterPositionInSpikesSpace * PushBackValue);
				}
			}
		}
	}
}

