// Fill out your copyright notice in the Description page of Project Settings.


#include "Curveball.h"
#include "GameFramework/ProjectileMovementComponent.h"

ACurveball::ACurveball()
{
	SpinRotator = FRotator(0, 1, 0);
	NumBlurMeshes = 6;
	BlurMeshAngle = 8;
	BlurMeshMaxOpacity = 0.4;
}

void ACurveball::BeginPlay()
{
	Super::BeginPlay();
	StartLocation = GetActorLocation();
	PrimaryActorTick.bCanEverTick = true;
	PrevX = GetActorLocation().X;
}

void ACurveball::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	PrevX = GetActorLocation().X;
}

void ACurveball::PhysicsTick()
{
	// Deceleration since last frame = feet traveled / 7 * 1 mph
	const float Deceleration = (PrevX - GetActorLocation().X) * 0.20952381622;
	PMC->Velocity += FVector(Deceleration, 0, 0);
	// Variable gravity scale
	if (GravityCurveFloat)
	{
		PMC->ProjectileGravityScale = GravityCurveFloat->GetFloatValue((1850 - PrevX) / 1850);
	}
}
