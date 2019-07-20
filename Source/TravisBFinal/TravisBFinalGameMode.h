// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TravisBFinalGameMode.generated.h"

UCLASS(minimalapi)
class ATravisBFinalGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATravisBFinalGameMode();

	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

	virtual void HandleCubes();

private:
	void HandleNewPlayer(APlayerController* NewPlayer);
};



