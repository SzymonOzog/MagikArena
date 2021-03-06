// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "UObject/ObjectMacros.h"

#include "MagikArenaGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class MAGIKARENA_API AMagikArenaGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintImplementableEvent)
	void Respawn(APlayerController* PlayerController);
};
