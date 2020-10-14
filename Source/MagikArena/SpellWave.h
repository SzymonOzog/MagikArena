// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseSpell.h"
#include "SpellWave.generated.h"

/**
 * 
 */
UCLASS()
class SLAUGHTERTHEVILLAGE_API ASpellWave : public ABaseSpell
{
	GENERATED_BODY()
public:
	ASpellWave();
	virtual void Tick(float DeltaTime) override;
	
protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* Mesh = nullptr;
	UPROPERTY(EditAnywhere)
	float PushBackStrength = 200.0f;
	UPROPERTY(EditAnywhere)
	float Speed = 10.0f;
	UPROPERTY(EditAnywhere)
	float DisappearSpeed = 10.0f;
	UPROPERTY(EditAnywhere)
	float TravelDistance = 3000.0f;
	FVector Destination;
	FVector DisappearDestination;
	bool bIsAtDestination = false;
	UPROPERTY(EditAnywhere)
	float ActivationAngleDegrees = 45.0f;
	
	UFUNCTION()
	void OnWaveBeginOverlap(AActor* OverpalledActor, AActor* OtherActor);
};
