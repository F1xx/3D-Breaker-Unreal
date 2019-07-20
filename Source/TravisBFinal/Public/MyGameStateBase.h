// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "MyGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class TRAVISBFINAL_API AMyGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	AMyGameStateBase();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Materials, Replicated)
		class UMaterialInterface* TeamOnePMaterials;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Materials, Replicated)
		class UMaterialInterface* TeamTwoPMaterials;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Gameplay, meta = (ClampMin = 0), Replicated)
		float TeamOneScore;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Gameplay, meta = (ClampMin = 0), Replicated)
		float TeamTwoScore;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Gameplay, Replicated)
		bool PlayerOneLoggedIn;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Gameplay, Replicated)
		bool PlayerTwoLoggedIn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Materials, Replicated)
		class UMaterialInterface* BoxColor1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Materials, Replicated)
		class UMaterialInterface* BoxColor2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Materials, Replicated)
		class UMaterialInterface* BoxColor3;

	UPROPERTY(BlueprintReadOnly, Category = State, Replicated)
		bool isMatchComplete = false;

	UFUNCTION()
		void EndGame();
protected:

	UPROPERTY(BlueprintReadOnly, Category = State, Replicated)
		FTimerHandle Turnoff;

	UFUNCTION(BlueprintCallable)
		virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		virtual void BeginPlay() override;	
};
