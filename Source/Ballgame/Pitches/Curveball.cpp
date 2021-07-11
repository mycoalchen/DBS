// Fill out your copyright notice in the Description page of Project Settings.


#include "Curveball.h"
#include "GameFramework/ProjectileMovementComponent.h"

ACurveball::ACurveball()
{
	SpinRotator = FRotator(1, 0, 0);
}

void ACurveball::BeginPlay()
{
	Super::BeginPlay();
	// PMC->ProjectileGravityScale = 0.3;
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
	// Deceleration since last frame
	const float Deceleration = (PrevX - GetActorLocation().X) * 0.4687; // 0.4687
	PMC->Velocity += FVector(Deceleration, 0, 0);
}
