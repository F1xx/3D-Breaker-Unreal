// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "TravisBFinalGameMode.h"
#include "TravisBFinalHUD.h"
#include "TravisBFinalCharacter.h"
#include "UObject/ConstructorHelpers.h"

#include "BoxActor.h"
#include <Kismet/GameplayStatics.h>

ATravisBFinalGameMode::ATravisBFinalGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ATravisBFinalHUD::StaticClass();
}

void ATravisBFinalGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
	HandleNewPlayer(NewPlayer);
}

void ATravisBFinalGameMode::HandleCubes()
{
	TArray<AActor*> Cubes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABoxActor::StaticClass(), Cubes);

	//for each actor in the array
	for (auto a : Cubes)
	{
		ABoxActor* box = Cast<ABoxActor>(a);

		box->AssignColor();
	}
}

void ATravisBFinalGameMode::HandleNewPlayer(APlayerController* NewPlayer)
{
	ATravisBFinalCharacter* character = Cast<ATravisBFinalCharacter>(NewPlayer->GetPawn());
	if (character)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Blue, "Character Logged In with team: " + FString::SanitizeFloat(character->Team));
		character->AssignTeams();
	}
}
