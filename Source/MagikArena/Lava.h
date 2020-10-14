// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Lava.generated.h"

UCLASS()
class SLAUGHTERTHEVILLAGE_API ALava : public AActor
{
	GENERATED_BODY()
	
public:	
	ALava();
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere)
	float DamagePerSecond = 30.0f;
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root = nullptr;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh = nullptr;
	UPROPERTY(EditAnywhere)
	UParticleSystem* Fire = nullptr;
	UFUNCTION()
    virtual void OnLavaBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

protected:
	virtual void BeginPlay() override;

private:
	struct FTimerHandle TimerHandle;
	UFUNCTION()
	void DealLavaDamage();
};
