// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BallBase.h"
#include "Fastball.generated.h"

UCLASS()
class BALLGAME_API AFastball : public ABallBase
{
	GENERATED_BODY()

	AFastball();
	
protected:
	virtual void BeginPlay() override;

	FVector StartLocation;
	float PrevX; // Previous x-coordinate
	
public:

	virtual void Tick(float DeltaTime) override;
	virtual void PhysicsTick() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics stats")
		float SpinRateRPM = 2300;

};
