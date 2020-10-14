// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Explosion.generated.h"

UCLASS()
class SLAUGHTERTHEVILLAGE_API AExplosion : public AActor
{
	GENERATED_BODY()
	
public:	
	AExplosion();
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
       virtual void OnExplosionOverlap(AActor* OverlappedActor, AActor* OtherActor);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* Mesh = nullptr;
	UPROPERTY(EditAnywhere)
	FVector StartScale = { 0.2f, 0.2f, 0.2f };
	UPROPERTY(EditAnywhere)
	FVector EndScale = { 5.0f, 5.0f, 5.0f };
	UPROPERTY(EditAnywhere)
	float GrowSpeed = 10.0f;
	UPROPERTY(EditAnywhere)
	float Damage = 10.0f;
};
