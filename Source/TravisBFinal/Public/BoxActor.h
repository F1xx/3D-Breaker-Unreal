// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "BoxActor.generated.h"

/**
 * 
 */
UCLASS()
class TRAVISBFINAL_API ABoxActor : public AStaticMeshActor
{
	GENERATED_BODY()
public:
		ABoxActor();

		virtual void BeginPlay() override;
	
	UFUNCTION(NetMulticast, Reliable)
		void Multicast_DestroyCubes();

	UFUNCTION()
		void FindNeighborCubes(AActor* cube, TArray<AActor*>& cubes);

	void FindNeighborArray(AActor* cube, TArray<AActor*>& cubes, TSubclassOf<AActor> type);

	void AssignColor();

	void Die();

	UFUNCTION(NetMulticast, Reliable)
		void Multicast_AssignColor();

	UFUNCTION(BlueprintCallable)
		void SetColor();

	UFUNCTION(BlueprintCallable)
		void PostBeginPlay();

	UPROPERTY(Replicated)
		FTimerHandle PostBeginPlayDelay;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Materials, Replicated)
		class UMaterialInterface* DefaultMaterial;

	UPROPERTY(Replicated)
		int BoxColor;

	UPROPERTY(Replicated)
		int ScoreToGive;

	class AMyGameStateBase* GetGameState();

protected:
	UPROPERTY(Replicated)
		TArray<ABoxActor*>Neighbors;
};
