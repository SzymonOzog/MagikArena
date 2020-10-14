// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseSpell.h"
#include "SpellOilStain.generated.h"

/**
 * 
 */
UCLASS()
class SLAUGHTERTHEVILLAGE_API ASpellOilStain : public ABaseSpell
{
	GENERATED_BODY()
public:
	ASpellOilStain();
	virtual void Tick(float DeltaTime) override;
	
	protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ADecalActor> OilStainClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ADecalActor> BurningOilStainClass;
	UPROPERTY()
	ADecalActor* Decal = nullptr;
	UPROPERTY(EditAnywhere)
	float SlowStrength = 2.0f;
	struct FTimerHandle BurnTimerHandle;
	
	UFUNCTION()
	void OnOilBeginOverlap(AActor* OverpalledActor, AActor* OtherActor);
	UFUNCTION()
    void OnOilEndOverlap(AActor* OverpalledActor, AActor* OtherActor);
	void Burn();
};
