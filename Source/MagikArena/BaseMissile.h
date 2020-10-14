// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseMissile.generated.h"

UCLASS()
class SLAUGHTERTHEVILLAGE_API ABaseMissile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseMissile();
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
        virtual void OnMissileHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION()
        virtual void applySpecialEffect(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit) {}
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UStaticMeshComponent* MissileMesh = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	class UProjectileMovementComponent* MissileMovement = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float MissileSpeed = 4000.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float GravityScale = 0.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	float Damage = 20.0f;
	class ABaseCharacter* OwningPlayer = nullptr;
};
