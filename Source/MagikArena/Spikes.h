// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Spikes.generated.h"

UCLASS()
class SLAUGHTERTHEVILLAGE_API ASpikes : public AActor
{
	GENERATED_BODY()
	
public:	
	ASpikes();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	
private:	
	UPROPERTY(EditAnywhere)
	float Damage = 999.0f;
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root = nullptr;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh = nullptr;
	UPROPERTY(EditAnywhere)
	float ActivationAngleDegrees = 45.0f;
	UPROPERTY(EditAnywhere)
	float PushBackValue = 50.0f;
	UPROPERTY(EditAnywhere)
	float TimeBetweenHits = 0.2f;

	UFUNCTION()
    virtual void OnSpikesHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);
};
