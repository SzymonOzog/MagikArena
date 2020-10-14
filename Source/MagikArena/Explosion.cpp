// Fill out your copyright notice in the Description page of Project Settings.


#include "Explosion.h"
#include "BaseCharacter.h"

// Sets default values
AExplosion::AExplosion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

}

// Called when the game starts or when spawned
void AExplosion::BeginPlay()
{
	Super::BeginPlay();
	OnActorBeginOverlap.AddDynamic(this, &AExplosion::OnExplosionOverlap);
	SetActorScale3D(StartScale);
}

// Called every frame
void AExplosion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SetActorScale3D(FMath::VInterpTo(GetActorScale3D(), EndScale, DeltaTime, GrowSpeed));
	if (GetActorScale3D() == EndScale)
	{
		Destroy();
	}
}

void AExplosion::OnExplosionOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (ABaseCharacter* Character = Cast<ABaseCharacter>(OtherActor))
	{
		Character->TakeDamage(Damage, FDamageEvent(), OverlappedActor->GetInstigatorController(), OverlappedActor);
	}
}