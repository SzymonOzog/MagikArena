// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseSpell.h"
#include "SpellMeteor.generated.h"

/**
 * 
 */
UCLASS()
class SLAUGHTERTHEVILLAGE_API ASpellMeteor : public ABaseSpell
{
	GENERATED_BODY()
public:
	ASpellMeteor();
	virtual void Tick(float DeltaTime) override;
	
protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(EditDefaultsOnly)
	float SpawnHeight = 2000.0f;
	UPROPERTY(EditDefaultsOnly)
	float Size = 3.0f;
	UPROPERTY(EditDefaultsOnly)
	float Speed = 3.0f;
	UPROPERTY(EditAnywhere)
	class UDestructibleComponent* DestructibleComponent = nullptr;
	
	UFUNCTION()
	void OnMeteorHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);
};
