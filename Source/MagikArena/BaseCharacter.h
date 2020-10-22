// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

UCLASS()
class MAGIKARENA_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();
		
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	UFUNCTION(BlueprintPure)
    inline bool IsPlayerDead() const { return CurrentHealth <= 0; }
	UFUNCTION(BlueprintPure)
    inline float GetHealthPercent() const { return CurrentHealth / MaxHealth; }
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	UFUNCTION(Server, Reliable)
	void ServerAttack();
	void ServerAttack_Implementation();
	UFUNCTION(Server, Reliable)
	void ServerRotateToControllerYaw();
	void ServerRotateToControllerYaw_Implementation();
	inline bool IsCharacterAttacking() const { return bIsAttacking; }
	UFUNCTION(Server, Reliable)
	void ServerPushBack(FVector PushDirection, float Duration = 0.2f);
	void ServerPushBack_Implementation(FVector PushDirection, float Duration = 0.2f);
	void StopPushBack() const;
	void HitSpikes();
	inline float GetSpikeHitTime() const { return SpikeHitTime; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


private:
	void SetupCharacterMovement();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastHandleDeath();
	void MulticastHandleDeath_Implementation();
	UFUNCTION(Server, Reliable)
	void ServerStopAttacking();
	void ServerStopAttacking_Implementation();
	void RotateSpellIndicator(float DeltaTime);
	void SetSpellIndicatorLocation() const;
	void SetupMissileSpawnParams();
	FTransform CalculateMissileSpawnTransform() const;
	void AimSpell();
	UFUNCTION(Client, Reliable)
	void ClientCastSpell();
	void ClientCastSpell_Implementation();
	UFUNCTION(Server, Reliable)
	void ServerCastSpell(FVector SpawnLocation);
	void ServerCastSpell_Implementation(FVector SpawnLocation);
	void MovementAbility();
	UFUNCTION(Server,Reliable)
	virtual void ServerCastMovementAbility();
	virtual void ServerCastMovementAbility_Implementation() {}
	void GetLifetimeReplicatedProps( TArray< FLifetimeProperty > & OutLifetimeProps ) const;
	
	UPROPERTY(EditDefaultsOnly)
	class UCameraComponent* Camera = nullptr;
	UPROPERTY(EditDefaultsOnly)
    class USpringArmComponent* SpringArm = nullptr;
	UPROPERTY(EditDefaultsOnly)
	float Speed = 1.0f;
	UPROPERTY(EditDefaultsOnly)
	float AirControl = 0.5f;
	UPROPERTY(EditDefaultsOnly)
	float JumpHeight = 600.0f;
	UPROPERTY(EditDefaultsOnly)
	float MaxHealth = 100.0f;
	UPROPERTY(VisibleAnywhere)
	float CurrentHealth;
	float SpikeHitTime;
	float FrictionFactor;
	UPROPERTY(Replicated)
	bool bIsAttacking = false;
	struct FActorSpawnParameters MissileSpawnParams;
	FRotator SpellIndicatorRotator = FRotator(0.0f, 0.0f, 0.0f);
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABaseMissile> MissileClass;
	float MissileSpawnDistance = 150.0f;
	UPROPERTY(EditAnywhere)
	float MissileCooldown = 0.4f;
	float MissileLastCast = 0.0f;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ADecalActor> SpellIndicatorClass;
	UPROPERTY()
	ADecalActor* SpellIndicator = nullptr;
	bool bIsAimingSpell = false;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABaseSpell> SpellClass;
	UPROPERTY(EditAnywhere)
	float MovementAbilityCooldown = 4.0f;
	float MovementAbilityLastCast = 0.0f;
};