// Fill out your copyright notice in the Description page of Project Settings.

#include "MyGameStateBase.h"

#include "Net/UnrealNetwork.h"
#include "TravisBFinalGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "BoxActor.h"



AMyGameStateBase::AMyGameStateBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMyGameStateBase::EndGame()
{
	//FGenericPlatformMisc::RequestExit(false);
}

void AMyGameStateBase::Tick(float DeltaTime)
{
	TArray<AActor*> cubes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABoxActor::StaticClass(), cubes);

	if (cubes.Num() == 0)
	{
		isMatchComplete = true;
		GetWorld()->GetTimerManager().SetTimer(Turnoff, this, &AMyGameStateBase::EndGame, 5.0f);
	}
}

//onces the match has started tells the gamemode to handle the cubes (ie assign their colors)
void AMyGameStateBase::BeginPlay()
{
	Super::BeginPlay();

	ATravisBFinalGameMode* mode = Cast<ATravisBFinalGameMode>(GetWorld()->GetAuthGameMode());

	if (mode)
	{
		mode->HandleCubes();
	}
}

void AMyGameStateBase::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//REPLICATE 
	DOREPLIFETIME(AMyGameStateBase, PlayerOneLoggedIn);
	DOREPLIFETIME(AMyGameStateBase, PlayerTwoLoggedIn);

	DOREPLIFETIME(AMyGameStateBase, TeamOneScore);
	DOREPLIFETIME(AMyGameStateBase, TeamTwoScore);

	DOREPLIFETIME(AMyGameStateBase, TeamOnePMaterials);
	DOREPLIFETIME(AMyGameStateBase, TeamTwoPMaterials);

	DOREPLIFETIME(AMyGameStateBase, BoxColor1);
	DOREPLIFETIME(AMyGameStateBase, BoxColor2);
	DOREPLIFETIME(AMyGameStateBase, BoxColor3);
	DOREPLIFETIME(AMyGameStateBase, isMatchComplete);
	DOREPLIFETIME(AMyGameStateBase, Turnoff);
}