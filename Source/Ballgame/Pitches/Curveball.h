// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BallBase.h"
#include "Curveball.generated.h"

UCLASS()
class BALLGAME_API ACurveball : public ABallBase
{
	GENERATED_BODY()

	ACurveball();
	
protected:
	virtual void BeginPlay() override;

	FVector StartLocation;
	float PrevX; // Previous x-coordinate

public:

	virtual void Tick(float DeltaTime) override;
	virtual void PhysicsTick() override;
	
};
