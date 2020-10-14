// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ExplosiveBarrel.generated.h"

UCLASS()
class SLAUGHTERTHEVILLAGE_API AExplosiveBarrel : public AActor
{
	GENERATED_BODY()
	
public:	
	AExplosiveBarrel();
	UFUNCTION()
	virtual void OnBarrelOverlap(AActor* OverlappedActor, AActor* OtherActor);

protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AExplosion> ExplosionClass;
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* Mesh = nullptr;
};
