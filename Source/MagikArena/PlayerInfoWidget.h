// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerInfoWidget.generated.h"

/**
 * 
 */
UCLASS()
class MAGIKARENA_API UPlayerInfoWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly)
	class ABaseCharacter* RepresentedCharacter;
};
