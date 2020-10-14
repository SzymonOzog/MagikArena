// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SLAUGHTERTHEVILLAGE_API ABaseWeapon : public AActor
{
	GENERATED_BODY()

public:	
	ABaseWeapon();
	virtual void Tick(float DeltaTime) override;
	virtual void Attack();

protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root = nullptr;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh = nullptr;
	UPROPERTY(EditDefaultsOnly)
	float Damage = 30.0f;
	float Range;
};

