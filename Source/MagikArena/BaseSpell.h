// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseSpell.generated.h"

UCLASS()
class MAGIKARENA_API ABaseSpell : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseSpell();
	virtual void Tick(float DeltaTime) override;
	inline float GetCastingRange() const { return CastingRange; }
	inline float GetCooldown() const { return Cooldown; }
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root = nullptr;
	UPROPERTY(EditAnywhere)
	float Damage = 0.0f;
	
private:
	UPROPERTY(EditDefaultsOnly)
	float Cooldown = 8.0f;
	UPROPERTY(EditAnywhere)
	float CastingRange = 5000.0f;
};
