// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerCharacter.h"
#include "ZoneController.generated.h"

/**
 * 
 */
UCLASS()
class BALLGAME_API AZoneController : public APlayerCharacter
{
	GENERATED_BODY()

public:
	UFUNCTION()
		void OnBallWallHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
