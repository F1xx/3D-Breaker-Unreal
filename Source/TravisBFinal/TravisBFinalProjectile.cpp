// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "TravisBFinalProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "BoxActor.h"
#include "TravisBFinalCharacter.h"

ATravisBFinalProjectile::ATravisBFinalProjectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &ATravisBFinalProjectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;

	SetReplicateMovement(true);
	SetReplicates(true);
}

void ATravisBFinalProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (Role == ROLE_Authority)
	{
		// Only add impulse and destroy projectile if we hit a physics
		if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && OtherComp->IsSimulatingPhysics())
		{

			ABoxActor* box = Cast<ABoxActor>(OtherActor);

			if (box)
			{
				//LET THE SERVER give the score and break the cubes
				box->Multicast_DestroyCubes();
				//give 1 extra score for this cube itself
				Cast<ATravisBFinalCharacter>(GetOwner())->GiveScore(box->ScoreToGive);
				//die when done
				box->Die();
			}


			OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());

			Destroy();
		}
	}
}