// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseMissile.h"
#include "AirMissile.generated.h"

/**
 * 
 */
UCLASS()
class SLAUGHTERTHEVILLAGE_API AAirMissile : public ABaseMissile
{
	GENERATED_BODY()
public:
	virtual void applySpecialEffect(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit) override;
private:
	UPROPERTY(EditDefaultsOnly)
	float PushStrength = 100.0f;
	FVector calculateMoveAmount(const FVector& playerLocation, const FVector& enemyLocation) const;


};
