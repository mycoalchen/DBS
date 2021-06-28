// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Strikezone.h"
#include "BallWall.generated.h"

UCLASS()
class BALLGAME_API ABallWall : public AStrikezone
{
	GENERATED_BODY()

		virtual void BeginPlay() override;

	UFUNCTION()
		void OnOverlapBegin2(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
