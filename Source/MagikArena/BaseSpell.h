// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseSpell.generated.h"

UCLASS()
class SLAUGHTERTHEVILLAGE_API ABaseSpell : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseSpell();
	virtual void Tick(float DeltaTime) override;
	inline float GetCastingRange() const { return CastingRange; }
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root = nullptr;
	UPROPERTY(EditAnywhere)
	float Damage = 0.0f;

private:
	UPROPERTY(EditAnywhere)
	float CastingRange = 5000.0f;
};
