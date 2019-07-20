// Fill out your copyright notice in the Description page of Project Settings.

#include "BoxActor.h"
#include "TravisBFinalProjectile.h"
#include "TravisBFinalCharacter.h"

#include "Net/UnrealNetwork.h"
#include "Engine/StaticMeshActor.h"
#include <Kismet/GameplayStatics.h>
#include "MyGameStateBase.h"

ABoxActor::ABoxActor()
{
	GetStaticMeshComponent()->SetSimulatePhysics(true);
	GetStaticMeshComponent()->SetGenerateOverlapEvents(true);
	GetStaticMeshComponent()->SetNotifyRigidBodyCollision(true);

	SetReplicates(true);
	SetReplicateMovement(true);
}

void ABoxActor::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(PostBeginPlayDelay, this, &ABoxActor::PostBeginPlay, 1.0f, false);
}

void ABoxActor::Multicast_DestroyCubes_Implementation()
{
	TArray<AActor*> Cubes;
	GetOverlappingActors(Cubes, ABoxActor::StaticClass());
	//FindNeighborArray(this, Cubes, ABoxActor::StaticClass());

	FindNeighborCubes(this, Cubes);

	for (int i = 0; i < Cubes.Num(); i++)
	{
		Neighbors.AddUnique(Cast<ABoxActor>(Cubes[i]));
	}

	for (auto a : Neighbors)
	{
		a->Die();
		ScoreToGive++;
	}
}

//loads the Neighbors TArray with its neighbors
void ABoxActor::FindNeighborCubes(AActor* cube, TArray<AActor*>& cubes)
{
	ABoxActor* box = Cast<ABoxActor>(cube);
	
	TArray<AActor*> morecubes;
	box->FindNeighborArray(box, morecubes, ABoxActor::StaticClass());

	for (auto a : morecubes)
	{
		ABoxActor* c = Cast<ABoxActor>(a);
		if (c->BoxColor == box->BoxColor)
		{
			int32 Index;
			//if we already found this
			if (cubes.Find(c, Index))
			{
				continue;
			}
			cubes.AddUnique(c);
			FindNeighborCubes(c, cubes);
		}
	}


}

void ABoxActor::FindNeighborArray(AActor* cube, TArray<AActor*>& cubes, TSubclassOf<AActor> type)
{
	TArray<AActor*> Cubes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), type, Cubes);

	//ABoxActor* box = Cast<ABoxActor>(cube);

	for (auto a : Cubes)
	{
		float dist = FVector::Dist(a->GetActorLocation(), cube->GetActorLocation());
// 		FVector origin = FVector::ZeroVector;
// 		FVector extent = FVector::ZeroVector;
// 		float radius;
// 
// 		UKismetSystemLibrary::GetComponentBounds((USceneComponent*)box->GetStaticMeshComponent(), origin, extent, radius);
		//float size = box->GetStaticMeshComponent()->GetComponentBounds()

		if (dist < 120.0f)
		{
			cubes.AddUnique(a);
			dist += 5;
		}
	}
}

//assigns the variable for the color to a random number between 0 and 2 (for 3 colors)
//Colors actually applied after Multicast_AssignColor calls SetColor
void ABoxActor::AssignColor()
{
	if (!GetGameState())
		return;

	BoxColor = FMath::RandRange(0, 2);
}

void ABoxActor::Die()
{
	Destroy();
}

void ABoxActor::SetColor()
{
	GetStaticMeshComponent()->SetMaterial(0, DefaultMaterial);
}

void ABoxActor::PostBeginPlay()
{
	if (Role == ROLE_Authority)
		Multicast_AssignColor();
}

class AMyGameStateBase* ABoxActor::GetGameState()
{
	return Cast<AMyGameStateBase>(GetWorld()->GetGameState());
}

void ABoxActor::Multicast_AssignColor_Implementation()
{
	if (GetGameState())
	{
		if (BoxColor == 0)
		{
			DefaultMaterial = GetGameState()->BoxColor1;
		}
		else if (BoxColor == 1)
		{
			DefaultMaterial = GetGameState()->BoxColor2;
		}
		else if (BoxColor == 2)
		{
			DefaultMaterial = GetGameState()->BoxColor3;
		}
		SetColor();
	}
}

void ABoxActor::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//REPLICATE 
	DOREPLIFETIME(ABoxActor, PostBeginPlayDelay);
	DOREPLIFETIME(ABoxActor, DefaultMaterial);
	DOREPLIFETIME(ABoxActor, Neighbors);
	DOREPLIFETIME(ABoxActor, BoxColor);
	DOREPLIFETIME(ABoxActor, ScoreToGive);
}