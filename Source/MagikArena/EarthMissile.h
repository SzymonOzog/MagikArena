// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseMissile.h"
#include "EarthMissile.generated.h"

/**
 * 
 */
UCLASS()
class SLAUGHTERTHEVILLAGE_API AEarthMissile : public ABaseMissile
{
	GENERATED_BODY()
	virtual void applySpecialEffect(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit) override;
	UPROPERTY(EditAnywhere)
	float SlowStrength = 1.5f;
	UPROPERTY(EditAnywhere)
	float SlowDuration = 1.0f;
	
};
