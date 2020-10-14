// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseMissile.h"
#include "FireMissile.generated.h"

/**
 * 
 */
UCLASS()
class SLAUGHTERTHEVILLAGE_API AFireMissile : public ABaseMissile
{
	GENERATED_BODY()
	virtual void applySpecialEffect(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit) override;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AExplosion> ExplosionClass;
};
